#include "includes/parser.h"

void fallback_message(char *message)
{
	write(2, message, ft_strlen(message));
}

#define CLAMP clamp
static inline float clamp(float value)
{
	return (value > 0.999) ? 0.999 : (value < 0.0) ? 0.0 : value;
}

int on_close(struct s_raytracer *rt)
{
	if (!rt)
		return (exit(0), 1);

	printf("P3\n%d %d\n255\n", rt->win.width, rt->win.height);
	for (int y = 0; y < rt->win.height; y++) {
		for (int x = 0; x < rt->win.width; x++) {
			int idx		 = y * rt->win.width + x;
			t_vec3 color = VMUL(1.0 / rt->nsamples, rt->acumm[idx]);
			printf("%d %d %d\n", (int)(CLAMP(sqrt(color.x)) * 255.99),
				   (int)(CLAMP(sqrt(color.y)) * 255.99),
				   (int)(CLAMP(sqrt(color.z)) * 255.99));
		}
	}

	if (rt->win.frame)
		mlx_destroy_image(rt->mlx, rt->win.frame);
	if (rt->win.ptr)
		mlx_destroy_window(rt->mlx, rt->win.ptr);
	mlx_destroy_display(rt->mlx);
	free(rt->scene.surfaces);
	free(rt->acumm);
	free(rt->mlx);
	exit(0);
	return (1);
}

#define MAX_SAMPLES 10

#define WRITE_COLOR(vec) ((int)vec.x << 16 | (int)vec.y << 8 | (int)vec.z)

int mlx_pixel_put_to_image(t_img *image, int x, int y, t_color3 color)
{
	char *dst;

	dst = image->data + y * image->size_line + x * image->abpp / 8;
	*(unsigned int *)dst = ((int)(CLAMP(sqrt(color.x)) * 256) << 16) |
						   ((int)(CLAMP(sqrt(color.y)) * 256) << 8) |
						   ((int)(CLAMP(sqrt(color.z)) * 256) << 0);
	return 0;
}

#include <time.h>

static inline float rand_float(float min, float max)
{
	return min + (max - min) * rand() / (RAND_MAX + 1.0);
}
static inline t_vec3 rand_vec(float min, float max)
{
	return (t_vec3){rand_float(min, max), rand_float(min, max),
					rand_float(min, max)};
}

static inline t_vec3 rand_unit_vec()
{
	while (true) {
		t_vec3 vec	= rand_vec(-1, 1);
		float lensq = VLENGHT_SQUARED(vec);
		if (1e-6 < lensq && lensq <= 1)
			return VMUL(1.0 / lensq, vec);
	}
}

static inline t_vec3 rand_on_hemisphere(const t_vec3 normal)
{
	t_vec3 unit_vec = rand_unit_vec();
	if (VDOT(unit_vec, normal) > 0.0)
		return unit_vec;
	return VMUL(-1, unit_vec);
}

int on_render(struct s_raytracer *rt)
{
	t_vec3 dir;
	t_hit hit;
	int x;
	int y;

	clock_t current_time = clock();

	float aratio = (float)rt->win.width / (float)rt->win.height;

	y	= 0;
	hit = ((t_hit){(t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});
	while (y < rt->win.height) {
		x = 0;
		while (x < rt->win.width) {
			dir = VSUB(VADD(VADD(rt->scene.camera.viewport.px00loc,
								 VMUL(x + rand_float(-0.5, 0.5),
									  rt->scene.camera.viewport.delta_u)),
							VMUL(y + rand_float(-0.5, 0.5),
								 rt->scene.camera.viewport.delta_v)),
					   rt->scene.camera.pos);

#define MAX_DEPTH 10
			float a			   = 0.5 * (dir.y + 1.0);
			t_vec3 pixel_color = VADD(VMUL(1 - a, ((t_color3){1.0, 1.0, 1.0})),
									  VMUL(a, ((t_color3){0.5, 0.7, 1.0})));
			t_vec3 origin	   = rt->scene.camera.pos;
			if (rt->scene.surfaces->hit_function) {
				for (int i = 0; i < MAX_DEPTH; i++) {
					hit = rt->scene.surfaces->hit_function(rt->scene.surfaces,
														   origin, dir);
					if (hit.t == INF)
						break;
					pixel_color = VMUL(0.1, pixel_color);
					dir	   = VADD(hit.normal, rand_on_hemisphere(hit.normal));
					origin = hit.p;
				}
			}

			rt->acumm[y * rt->win.width + x] =
				VADD(rt->acumm[y * rt->win.width + x], pixel_color);

			mlx_pixel_put_to_image(
				rt->win.frame, x, y,
				VMUL(1.0 / rt->nsamples, rt->acumm[y * rt->win.width + x]));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt->mlx, rt->win.ptr, rt->win.frame, 0, 0);

	char name[100];
	sprintf(name, "%i sample %fms", rt->nsamples++,
			((current_time - clock()) / (float)CLOCKS_PER_SEC) * 1000.0);
	XStoreName(rt->mlx->display, rt->win.ptr->window, name);
	return (0);
}

#define TO_RAD(deg) (deg * M_PI / 180)
#define VCROSS(a, b)           \
	((t_vec3){                 \
		a.y * b.z - a.z * b.y, \
		a.z * b.x - a.x * b.z, \
		a.x * b.y - a.y * b.x, \
	})
#define ASPECT_RATIO (16.0 / 9.0)

int init_camera(struct s_camera *camera, int width, int height)
{
	float aspect_ratio;
	float viewport_height;
	float viewport_width;
	float focal_lenght;

	aspect_ratio	= width / (float)height;
	focal_lenght	= VLENGHT(VSUB(camera->pos, camera->w));
	camera->fov		= tan(camera->fov * 0.5 * M_PI / 180.0);
	viewport_width	= 2 * camera->fov * focal_lenght;
	viewport_height = viewport_width / aspect_ratio;
	// CAMERA BASE
	camera->w = VNORM(VSUB(camera->pos, camera->w));
	camera->v = ((t_vec3){0, 1, 0});
	if (camera->w.y == 1)
		camera->v = ((t_vec3){0, 0, 1});

	camera->u = VNORM(VCROSS(camera->v, camera->w));
	camera->v = VNORM(VCROSS(camera->w, camera->u));
	// VIEWPORT
	camera->viewport.delta_u = VMUL(viewport_width, camera->u);
	camera->viewport.delta_v = VMUL(-viewport_height, camera->v);
	camera->viewport.px00loc =
		VSUB(VSUB(VSUB(camera->pos, VMUL(focal_lenght, camera->w)),
				  VMUL(0.5, camera->viewport.delta_u)),
			 VMUL(0.5, camera->viewport.delta_v));
	camera->viewport.delta_u = VMUL(1.0 / width, camera->viewport.delta_u);
	camera->viewport.delta_v = VMUL(1.0 / height, camera->viewport.delta_v);
	camera->viewport.px00loc = VADD(
		camera->viewport.px00loc,
		VMUL(0.5, VADD(camera->viewport.delta_u, camera->viewport.delta_v)));
	return 0;
}

int main(int ac, char **av)
{
	struct s_raytracer rt;
	int fd;

	if (ac < 2)
		return (fallback_message(ERR_NO_FILE), -1);

	ft_bzero(&rt, sizeof(struct s_raytracer));

	rt.win.width	  = 1080;
	rt.win.height	  = rt.win.width / ASPECT_RATIO;
	rt.scene.surfaces = malloc(MAX_SURFACES * sizeof(t_surface));
	ft_bzero(rt.scene.surfaces, MAX_SURFACES * sizeof(t_surface));
	parse_objects(open(av[1], O_RDWR), parser_rules(&rt.scene));
	init_camera(&rt.scene.camera, rt.win.width, rt.win.height);

	rt.mlx		 = mlx_init();
	rt.win.ptr	 = mlx_new_window(rt.mlx, rt.win.width, rt.win.height, "3DRT");
	rt.win.frame = mlx_new_image(rt.mlx, rt.win.width, rt.win.height);

	rt.acumm = malloc(rt.win.height * rt.win.width * sizeof(t_vec3));
	ft_bzero(rt.acumm, rt.win.height * rt.win.width * sizeof(t_vec3));

	rt.nsamples = 1;

	mlx_hook(rt.win.ptr, DestroyNotify, StructureNotifyMask,
			 (int (*)(void *))on_close, &rt);
	mlx_loop_hook(rt.mlx, on_render, &rt);
	// on_render(&rt);
	mlx_loop(rt.mlx);
	return (0);
}
