
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <limits.h>
#include <math.h>
#include <mlx.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "minilibx-linux/mlx.h"
#include "minilibx-linux/mlx_int.h"

typedef enum { PROP_NONE, PROP_ROUGHNESS, PROP_METALLIC } t_prop;
typedef enum { INPUT_NORMAL, INPUT_WAIT_INDEX } t_input_mode;
typedef struct {
  t_input_mode mode;
  t_prop prop;
} t_ui;
typedef struct {
  float x, y, z;
} t_vec3d;

typedef struct {
  int w;
  int h;
  int bpp;
  int sline;
  int endian;
  char *addr;
  void *ptr;
} t_data;
typedef struct {
  t_vec3d albedo;
  float roughness;
  float metallic;

  t_vec3d emission_color;
  float emission_power;
} Material;
#define GET_EMIS(m) (m.emission_color * m.emission_power)

typedef struct {
  t_vec3d pos;
  float radius;

  Material mat;
} t_sphere;

typedef enum {
  CAMERA,
  LIGHT,
} t_select;

typedef struct {
  float hit_distance;
  t_vec3d world_normal;
  t_vec3d world_position;

  int object_index;
} t_hit_payload;

typedef struct {
  int nspheres;
  t_sphere *spheres;
  t_vec3d light;
  t_vec3d camera;
  t_select entity;

  clock_t last_time;
  t_hit_payload payload;
} t_scene;

typedef struct {
  t_data buffer;
  void *display;
  void *window;

  t_scene scene;
  t_ui ui;

  t_vec3d *accum;
  int samples;
} t_app;

int close_application(t_app *app);

int vec_to_color(t_vec3d vec) {
  // Clamp and pack as ARGB using unsigned ops
  float rx = fmaxf(fminf(vec.x, 1.0f), 0.0f);
  float ry = fmaxf(fminf(vec.y, 1.0f), 0.0f);
  float rz = fmaxf(fminf(vec.z, 1.0f), 0.0f);
  uint32_t r = (uint32_t)(rx * 255.0f) & 0xFFu;
  uint32_t g = (uint32_t)(ry * 255.0f) & 0xFFu;
  uint32_t b = (uint32_t)(rz * 255.0f) & 0xFFu;
  uint32_t a = 0xFFu;
  return (int)((a << 24) | (r << 16) | (g << 8) | b);
}

float dot_product(t_vec3d a, t_vec3d b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline t_vec3d normalize(t_vec3d v) {
  float mag = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
  if (mag <= 1e-8f) return (t_vec3d){0.f, 0.f, 0.f};
  return (t_vec3d){v.x / mag, v.y / mag, v.z / mag};
}

static inline t_vec3d subvec(t_vec3d a, t_vec3d b) {
  return (t_vec3d){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline t_vec3d addvec(t_vec3d a, t_vec3d b) {
  return (t_vec3d){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline t_vec3d hadamard(t_vec3d a, t_vec3d b) {
  return (t_vec3d){a.x * b.x, a.y * b.y, a.z * b.z};
}
static inline t_vec3d muls(t_vec3d a, float s) {
  return (t_vec3d){a.x * s, a.y * s, a.z * s};
}

int mlx_put_pixel_on_image(t_data *img, int x, int y, int color) {
  int alpha;
  char *dst;

  if (x <= 0 || x >= img->w || y <= 0 || y >= img->h) return 0;
  alpha = ((color >> 24) & 0xFF) / 0xFF;
  dst = img->addr + y * img->sline + x * (img->bpp / 8);
  *(unsigned *)dst = (int)((color >> 16) & 0xFF) * alpha << 16 |
                     (int)((color >> 8) & 0xFF) * alpha << 8 |
                     (int)((color) & 0xFF) * alpha;
  return (0);
}

int trace_ray(t_scene *scene, t_vec3d origin, t_vec3d dir) {
  t_vec3d oc;
  t_hit_payload *pd = &scene->payload;

  pd->object_index = -1;
  pd->hit_distance = __FLT_MAX__;
  for (int i = 0; i < scene->nspheres; i++) {
    t_sphere *sphere = &scene->spheres[i];
    oc = subvec(origin, sphere->pos);  // camera - center

    float a = dot_product(dir, dir);
    float b = 2.0f * dot_product(oc, dir);
    float c = dot_product(oc, oc) - sphere->radius * sphere->radius;

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) continue;

    float sD = sqrtf(discriminant);
    float t0 = (-b - sD) / (2.0f * a);
    float t1 = (-b + sD) / (2.0f * a);
    if (t0 <= 0.0f) t0 = t1;  // pick nearest positive
    if (t0 <= 0.0f) continue;

    if (t0 > 0.0f && t0 < pd->hit_distance) {
      pd->hit_distance = t0;
      pd->object_index = i;
    }
  }
  if (pd->object_index == -1) return (1);

  oc = subvec(origin, scene->spheres[pd->object_index].pos);

  pd->world_position = (t_vec3d){oc.x + dir.x * pd->hit_distance,
                                 oc.y + dir.y * pd->hit_distance,
                                 oc.z + dir.z * pd->hit_distance};

  pd->world_normal = normalize(pd->world_position);

  pd->world_position =
      addvec(pd->world_position, scene->spheres[pd->object_index].pos);
  return (0);
}
// static inline t_vec3d reflect(t_vec3d v, t_vec3d n) {
//   // Assumes n is normalized
//   float dn = dot_product(v, n);
//   return (t_vec3d){v.x - 2.0f * dn * n.x, v.y - 2.0f * dn * n.y,
//                    v.z - 2.0f * dn * n.z};
// }
#define BOUNCES 15
#define RANDRANGE(min, max)                                        \
  ((float)((double)(min) + (((double)rand()) / (double)RAND_MAX) * \
                               ((double)(max) - (double)(min))))

static inline t_vec3d norm_rand(float min, float max) {
  t_vec3d vec = {RANDRANGE(min, max), RANDRANGE(min, max), RANDRANGE(min, max)};
  return normalize(vec);
}

#define SKY_COLOR (t_vec3d){0.f, 0.f, 0.f}
static inline void reset_accumulation(t_app *app) { app->samples = 0; }

t_vec3d per_pixel(t_scene scene, t_vec3d dir) {
  t_vec3d light = (t_vec3d){0.f, 0.f, 0.f};
  t_vec3d contribution = (t_vec3d){1.f, 1.f, 1.f};
  t_vec3d origin = scene.camera;
  t_hit_payload *pd = &scene.payload;

  for (int i = 0; i < BOUNCES; i++) {
    if (trace_ray(&scene, origin, dir)) {
      light = addvec(light, hadamard(SKY_COLOR, contribution));
      break;
    }
    t_sphere *sphere = &scene.spheres[scene.payload.object_index];
    light = addvec(
        light, muls(sphere->mat.emission_color, sphere->mat.emission_power));
    contribution = hadamard(contribution, sphere->mat.albedo);
    origin = addvec(pd->world_position, muls(pd->world_normal, 0.0001f));
    dir = normalize(addvec(pd->world_normal, norm_rand(-1.0f, 1.0f)));
  }
  return light;
}

int on_update(t_app *app) {
  clock_t current_time = clock();
  double dt_sec =
      (double)(current_time - app->scene.last_time) / (double)CLOCKS_PER_SEC;
  app->scene.last_time = current_time;

  if (app->samples == 0) {
    memset(app->accum, 0, sizeof(t_vec3d) * app->buffer.w * app->buffer.h);
  }

  t_vec3d dir;
  for (int y = 0; y < app->buffer.h; y++) {
    for (int x = 0; x < app->buffer.w; x++) {
      // optional subpixel jitter:
      float jx = RANDRANGE(0.0f, 1.0f);
      float jy = RANDRANGE(0.0f, 1.0f);

      dir.x = (((x + jx) / (float)app->buffer.w) * 2.f - 1.f);
      dir.y = (1.f - (y + jy) / (float)app->buffer.h) * 2.f - 1.f;
      dir.z = -1.0f;

      t_vec3d sample = per_pixel(app->scene, normalize(dir));
      int idx = y * app->buffer.w + x;
      app->accum[idx] = addvec(app->accum[idx], sample);

      float inv = 1.0f / (float)(app->samples + 1);
      t_vec3d avg = muls(app->accum[idx], inv);
      mlx_put_pixel_on_image(&app->buffer, x, y, vec_to_color(avg));
    }
  }

  app->samples += 1;

  char title[128];
  snprintf(title, sizeof(title), "MiniRT - %.3f ms | SPP: %d", dt_sec * 1000.0,
           app->samples);
  XStoreName(((t_xvar *)app->display)->display,
             ((t_win_list *)app->window)->window, title);
  XFlush(((t_xvar *)app->display)->display);

  mlx_put_image_to_window(app->display, app->window, app->buffer.ptr, 0, 0);
  return 0;
}

int key_press(int keycode, t_app *app);
int main() {
  t_app app = (t_app){0};
  srand((unsigned)time(NULL));

  app.scene.nspheres = 2;
  app.scene.spheres = (t_sphere[]){
      [0] = {.pos = {0.f, 0.4f, 0.f},
             .radius = 0.5f,
             .mat = {.albedo = {0.8f, 0.3f, 1.f},
                     .roughness = 0.0f,
                     .metallic = 0.0f,
                     .emission_color = {1.f, 0.8f, 0.6f},
                     .emission_power = 3.f}},
      [1] = {.pos = {0.f, -10.1f, 0.f},
             .radius = 10.f,
             .mat = {.albedo = {0.2f, 0.3f, 1.f},
                     .roughness = 0.0f,
                     .metallic = 0.0f,
                     .emission_color = {0.f, 0.f, 0.f},
                     .emission_power = 0.f}},
  };

  app.buffer.w = 800;
  app.buffer.h = 800;
  app.samples = 0;

  app.ui.mode = INPUT_NORMAL;
  app.ui.prop = PROP_NONE;

  app.scene.entity = CAMERA;
  app.scene.camera = (t_vec3d){0.f, 0.f, 3.f};
  app.scene.light = (t_vec3d){0.f, 1.f, 1.f};
  app.display = mlx_init();
  app.window = mlx_new_window(app.display, app.buffer.w, app.buffer.h, "3D");

  app.buffer.ptr = mlx_new_image(app.display, app.buffer.w, app.buffer.h);
  app.buffer.addr = mlx_get_data_addr(app.buffer.ptr, &app.buffer.bpp,
                                      &app.buffer.sline, &app.buffer.endian);

  app.accum = calloc(app.buffer.w * app.buffer.h, sizeof(t_vec3d));
  if (!app.accum) {
    fprintf(stderr, "Failed to allocate accumulation buffer\n");
    return 1;
  }

  app.scene.last_time = clock();

  mlx_loop_hook(app.display, on_update, &app);
  mlx_hook(app.window, KeyPress, KeyPressMask, key_press, &app);
  mlx_hook(app.window, DestroyNotify, StructureNotifyMask, close_application,
           &app);
  mlx_loop(app.display);
  return (0);
}
int key_press(int keycode, t_app *app) {
  // Sequenced input: waiting for object index?
  if (app->ui.mode == INPUT_WAIT_INDEX) {
    if (keycode == XK_Escape) {
      app->ui.mode = INPUT_NORMAL;
      app->ui.prop = PROP_NONE;
      return 0;
    }
    if (keycode >= XK_0 && keycode <= XK_9) {
      int idx = keycode - XK_0;
      if (idx >= 0 && idx < app->scene.nspheres) {
        t_sphere *s = &app->scene.spheres[idx];
        switch (app->ui.prop) {
          case PROP_ROUGHNESS:
            s->mat.roughness += 0.1f;
            if (s->mat.roughness > 1.0f) s->mat.roughness = 0.0f;
            printf("Sphere %d roughness -> %.2f\n", idx, s->mat.roughness);
            reset_accumulation(app);
            break;
          case PROP_METALLIC:
            s->mat.metallic += 0.1f;
            if (s->mat.metallic > 1.0f) s->mat.metallic = 0.0f;
            printf("Sphere %d metallic -> %.2f\n", idx, s->mat.metallic);
            reset_accumulation(app);
            break;
          default:
            break;
        }
      } else {
        printf("Index %d out of range [0..%d]\n", idx, app->scene.nspheres - 1);
      }
      app->ui.mode = INPUT_NORMAL;
      app->ui.prop = PROP_NONE;
      return 0;
    }
    // Ignore other keys while waiting for index
    return 0;
  }

  // Normal mode
  t_vec3d *pos =
      (app->scene.entity == LIGHT) ? &app->scene.light : &app->scene.camera;

  if (keycode == XK_Escape) return close_application(app);
  if (keycode == XK_l) {
    app->scene.entity = LIGHT;
    pos = &app->scene.light;
  }
  if (keycode == XK_c) {
    app->scene.entity = CAMERA;
    pos = &app->scene.camera;
  }

  // Enter property selection -> next digit chooses sphere index
  if (keycode == XK_r) {
    app->ui.prop = PROP_ROUGHNESS;
    app->ui.mode = INPUT_WAIT_INDEX;
    printf("Select sphere index [0..%d] for ROUGHNESS\n",
           app->scene.nspheres - 1);
    return 0;
  }
  if (keycode == XK_m) {
    app->ui.prop = PROP_METALLIC;
    app->ui.mode = INPUT_WAIT_INDEX;
    printf("Select sphere index [0..%d] for METALLIC\n",
           app->scene.nspheres - 1);
    return 0;
  }

  // Movement (when not in selection mode)
  if (keycode == XK_w) {
    pos->z += 0.1f;
    reset_accumulation(app);
  }
  if (keycode == XK_s) {
    pos->z -= 0.1f;
    reset_accumulation(app);
  }
  if (keycode == XK_d) {
    pos->x += 0.1f;
    reset_accumulation(app);
  }
  if (keycode == XK_a) {
    pos->x -= 0.1f;
    reset_accumulation(app);
  }
  if (keycode == XK_q) {
    pos->y += 0.1f;
    reset_accumulation(app);
  }
  if (keycode == XK_e) {
    pos->y -= 0.1f;
    reset_accumulation(app);
  }

  printf("%i [%.3f %.3f %.3f]\n", app->scene.entity, pos->x, pos->y, pos->z);
  return 0;
}
int close_application(t_app *app) {
  free(app->accum);
  if (app->buffer.ptr) mlx_destroy_image(app->display, app->buffer.ptr);
  mlx_destroy_window(app->display, app->window);
  mlx_destroy_display(app->display);
  free(app->display);
  exit(0);
}