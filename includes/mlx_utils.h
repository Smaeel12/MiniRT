#pragma once

#include "../mlx/mlx_int.h"
#include "../Libft/libft.h"

#include "vectors.h"
#include "structs.h"

#include <time.h>

typedef int (*t_hook)(void *);
typedef int (*t_hook2)(int, void *);

int mlx_pixel_put_to_image(t_img *image, int x, int y, t_color3 color);
int on_close(struct s_raytracer *rt);
t_color3 per_pixel(struct s_scene *scene, t_vec3 org, t_vec3 dir);
int on_render(struct s_raytracer *rt);
