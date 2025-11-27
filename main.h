#pragma once

#include <limits.h>
#include <math.h>
#include <unistd.h>

#include "Libft/libft.h"
#include "mlx/mlx.h"
#include "mlx/mlx_int.h"

typedef union {
	struct {
		float x, y, z;
	};
	struct {
		float r, g, b;
	};
} t_vec3;

typedef t_vec3 t_color3;

union s_surface;

typedef struct {
	t_vec3 n;
	double t;
} t_hit;

typedef struct {
	t_hit (*hit_function)(union s_surface *, t_vec3, t_vec3);
	t_vec3 pos;
	float diameter;
	t_vec3 color;
} t_sphere;	 // SPHERE

typedef struct {
	t_hit (*hit_function)(union s_surface *, t_vec3, t_vec3);
	t_vec3 pos;
	t_vec3 normal;
	t_vec3 color;
} t_plane;	// PLANE

typedef struct {
	t_hit (*hit_function)(union s_surface *, t_vec3, t_vec3);
	t_vec3 pos;
	t_vec3 normal;
	float diameter;
	float height;
	t_vec3 color;
} t_cylinder;  // CYLINDER

typedef union s_surface {
	t_hit (*hit_function)(union s_surface *, t_vec3, t_vec3);
	t_cylinder cylinder;
	t_sphere sphere;
	t_plane plane;
} t_surface;  // SURFACES

struct s_app {
	struct {
		int w, h;
		t_xvar *mlx;
		t_win_list *win;
		t_img *framebuffer;
	} window;

	struct s_scene {
		struct s_camera {
			t_vec3 pos;
			t_vec3 fd;
			int fov;
		} camera;  // CAMERA
		t_surface *surfaces;
	} scene;
};

#define INF INFINITY

t_hit sphere_intersection(t_sphere *sphere, t_vec3 org, t_vec3 dir);
t_hit plane_intersection(t_plane *plane, t_vec3 org, t_vec3 dir);
t_hit cylinder_intersection(t_cylinder *cylinder, t_vec3 org, t_vec3 dir);
int init_scene(struct s_app *rt, int file);

#define MAX_SURFACES 10

#define VEC3 vec3
static inline t_vec3 vec3(float x, float y, float z)
{
	return (t_vec3){{x, y, z}};
}

#define COLOR3 color3
static inline t_color3 color3(float r, float g, float b)
{
	return (t_color3){{r, g, b}};
}
