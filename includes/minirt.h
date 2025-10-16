#ifndef MINIRT_H
#define MINIRT_H

#include "mlx.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define KEY_ESC 65307
#define WIN_W 2000
#define WIN_H 1500

typedef struct s_app
{
	void *mlx;
	void *win;
	void *img;
	char *addr;
	int bpp;
	int ll;
	int endian;
	int w;
	int h;
} t_app;

int init_app(t_app *app, int w, int h, char *title);
int destroy_app(t_app *app);
int on_key(int keycode, t_app *app);
int on_close(t_app *app);

// func for drawing
void fill_black(t_app *app);
void put_frame(t_app *app);

/* ===== Scene types ===== */
typedef struct s_vec3
{
	double x;
	double y;
	double z;
} t_vec3;
typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

typedef struct s_ambient
{
	int has;
	double ratio;
	t_color color;
} t_ambient;
typedef struct s_camera
{
	int has;
	t_vec3 pos;
	t_vec3 dir;
	double fov;
} t_camera;
typedef struct s_light
{
	int has;
	t_vec3 pos;
	double bright;
	t_color color;
} t_light;

typedef struct s_sphere
{
	t_vec3 center;
	double diam;
	t_color color;
	struct s_sphere *next;
} t_sphere;

typedef struct s_scene
{
	t_ambient a;
	t_camera c;
	t_light l;
	t_sphere *spheres;
} t_scene;

/* ===== Parser API ===== */
int load_scene(const char *path, t_scene *sc);
void free_scene(t_scene *sc);

/* string -> values helpers */
int parse_vec3(const char *s, t_vec3 *v);
int parse_color(const char *s, t_color *c);
int parse_double(const char *s, double *out);
int parse_in_range_d(double v, double minv, double maxv);
int parse_in_range_i(int v, int mini, int maxi);
t_vec3 vnorm(t_vec3 v, int *ok);

#endif
