/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:24:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/11/15 01:44:41 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "minilibx-linux/mlx.h"
#include "minilibx-linux/mlx_int.h"
#include <math.h>
#include <time.h>

double	lenght_squared(t_vec3 vec)
{
	return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static inline double	rand_double(void)
{
	return (rand() / ((double)RAND_MAX + 1));
}

static inline t_vec3	rand_vec(void)
{
	return (VEC3(rand_double(), rand_double(), rand_double()));
}

static inline t_vec3	rand_unit_vec(void)
{
	t_vec3	uvec;
	double	lensq;

	while (1)
	{
		uvec = rand_vec();
		lensq = lenght_squared(uvec);
		if (1e-160 < lensq && lensq <= 1)
		{
			return (vmul(1 / sqrt(lensq), uvec));
		}
	}
}

// static inline t_vec3 rand_on_hemisphere(t_vec3 normal)
// {
// 	t_vec3 on_unit_sphere = rand_unit_vec();
// 	if (VDOT(on_unit_sphere, normal) <= 0.0)
// 		return (VMUL(-1, on_unit_sphere));
// 	return (on_unit_sphere);
// }

// static inline t_color3 linear_to_gamma(t_color3 linear_comps)
// {
// 	if (linear_comps.r >= 0.0)
// 		sqrt(linear_comps.r);
// 	if (linear_comps.g >= 0.0)
// 		sqrt(linear_comps.g);
// 	if (linear_comps.b >= 0.0)
// 		sqrt(linear_comps.b);
// 	return (linear_comps);
// }

int	mlx_put_pixel_on_image(t_img *img, t_color3 color, int x, int y)
{
	char	*dst;

	dst = img->data + (y * img->size_line + x * (img->abpp / 8));
	*(unsigned int *)dst = (int)(color.r * 255.999) << 16 | (int)(color.g
			* 255.999) << 8 | (int)(color.b * 255.999);
	return (0);
}

t_hit	sphere_intersection(t_sphere *sphere, t_vec3 org, t_vec3 dir)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	disc;
	double	t;

	oc = VSUB(sphere->pos, org);
	a = VDOT(dir, dir);
	b = -2.0 * VDOT(dir, oc);
	disc = b * b - 4 * a * (VDOT(oc, oc) - sphere->radius * sphere->radius);
	if (disc < 0.0)
		return ((t_hit){INFINITY, VEC3(0, 0, 0), VEC3(0, 0, 0)});
	t = (-b - sqrt(disc)) / (2.0 * a);
	return ((t_hit){t, VADD(org, VMUL(t, dir)), VNORM(VSUB(VADD(org, VMUL(t,
						dir)), sphere->pos))});
}

t_color3	per_pixel(t_surface *surface, int max_depth, t_vec3 org, t_vec3 dir)
{
	int			i;
	t_hit		hit;
	t_vec3		norm_rep;
	t_hit		closest_hit;
	t_surface	*temp;
	t_vec3		unit_dir;
	double		a;
	t_vec3		direction;

	closest_hit = {.t;
	i = 0;
	closest_hit = {.t = INFINITY};
	temp = surface;
	if (max_depth <= 0)
		return (COLOR3(0, 0, 0));
	while (temp)
	{
		hit = temp->hit_func(temp, org, dir);
		if (hit.t > 0.001 && hit.t < closest_hit.t)
			closest_hit = hit;
		temp = temp->next;
		i++;
	}
	if (isinf(closest_hit.t))
	{
		unit_dir = VNORM(dir);
		a = 0.5 * (unit_dir.y + 1.0);
		return (VADD(VMUL((1.0 - a), VEC3(1.0, 1.0, 1.0)), VMUL(a, VEC3(0.5,
						0.7, 1.0))));
	}
	direction = VADD(closest_hit.n, rand_unit_vec());
	return (VMUL(0.1, per_pixel(surface, max_depth--, closest_hit.p,
				direction)));
	// return (VMUL(0.5, VADD(closest_hit.n, VEC3(1, 1, 1))));
}
inline t_color3	clamp(t_color3 color)
{
	return (t_color3){
		.r = color.r > 1.0 ? 1.0
			: color.r < 0  ? 0.0
							: color.r,
		.g = color.g > 1.0 ? 1.0
			: color.g < 0  ? 0.0
							: color.g,
		.b = color.b > 1.0 ? 1.0
			: color.b < 0  ? 0.0
							: color.b,
	};
}
#define HFOV 70.0
#define MAX_DEPTH 500
#define ASPECT_RATIO (16.0 / 9.0)
#define TO_RAD(deg) (deg * M_PI / 180.0)
#define NUM_OF_SAMPLES 500

int	on_update(t_mlx *mlx)
{
	t_surface	*surface;
	t_vec3		color;
	t_vec3		dir;
	int			y;
	int			x;
	int			fd;

	y = 0;
	fd = open("image.ppm", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	dprintf(fd, "P3\n%i %i\n255\n", mlx->w, mlx->h);
	while (y < mlx->h)
	{
		x = 0;
		while (x < mlx->w)
		{
			dir = VEC3(2.0 * ((x + 0.5) / mlx->w) - 1.0, 1.0 - 2.0 * ((y + 0.5)
						/ mlx->h), -1.0);
			dir = VADD(VADD(VMUL(dir.x, mlx->scene.camera.rt), VMUL(dir.y,
							mlx->scene.camera.up)), VMUL(dir.z,
						mlx->scene.camera.fd));
			color = per_pixel(mlx->scene.surfaces, 1, mlx->scene.camera.pos,
					dir);
			mlx_put_pixel_on_image(mlx->buffer, color, x, y);
			// t_vec3 pixel_color = COLOR3(0.0, 0.0, 0.0);
			// for (int sample = 0; sample < NUM_OF_SAMPLES; sample++) {
			// 	dir = VEC3((2.0 * ((x + rand_double()) / (double)mlx->w)
			// - 1.0), 				(1.0 - 2.0 * ((y + rand_double()) /
			// (double)mlx->h)), 				-1.0);
			// 	color		= per_pixel(mlx->scene.surface, MAX_DEPTH,
			// 							mlx->scene.camera, dir);
			// 	pixel_color = VADD(pixel_color, color);
			// }
			// pixel_color = clamp(VMUL(1.0 / NUM_OF_SAMPLES, pixel_color));
			// pixel_color = linear_to_gamma(pixel_color);
			// dprintf(fd, "%i %i %i\n", (int)(pixel_color.r * 255.9),
			// 		(int)(pixel_color.g * 255.9), (int)(pixel_color.b *
			// 255.9));
			// mlx_put_pixel_on_image(mlx->buffer, pixel_color, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->buffer, 0, 0);
	return (0);
}

int	on_close(t_mlx *mlx)
{
	mlx_destroy_image(mlx->ptr, mlx->buffer);
	mlx_destroy_window(mlx->ptr, mlx->win);
	mlx_destroy_display(mlx->ptr);
	exit(0);
	return (0);
}

#define CAMERA_POS (VEC3(0, 0, 3.0))
#define CAMERA_DIR (VEC3(0.0, 0.0, 1.0))
#define CAMERA_FOV (70.0)

int	main(void)
{
	t_mlx	mlx;
	char	name[100];
	double	vw;
	double	vh;

	srand(time(NULL));
	mlx.w = 800;
	mlx.h = mlx.w / ASPECT_RATIO;
	sprintf(name, "%i %i", mlx.w, mlx.h);
	mlx.ptr = mlx_init();
	mlx.win = mlx_new_window(mlx.ptr, mlx.w, mlx.h, name);
	if (mlx.ptr == NULL || mlx.win == NULL)
		return (0);
	mlx.buffer = mlx_new_image(mlx.ptr, mlx.w, mlx.h);
	mlx.scene.camera.pos = CAMERA_POS;
	mlx.scene.camera.fd = VMUL(-1, VNORM(CAMERA_DIR));
	mlx.scene.camera.rt = VCROSS(mlx.scene.camera.fd, VEC3(0.0, 1.0, 0.0));
	mlx.scene.camera.up = VCROSS(mlx.scene.camera.fd, mlx.scene.camera.rt);
	vw = 2 * tan(TO_RAD(HFOV) / 2.0);
	vh = vw / ASPECT_RATIO;
	mlx.scene.surfaces = (t_surface *)&(t_sphere){(t_hit(*)(struct s_surface *,
				t_vec3, t_vec3))sphere_intersection, NULL, COLOR3(1, 0, 0),
		VEC3(0, 0, -5), 1};
	// mlx.scene.surfaces->next = (t_surface *)&(t_sphere){
	// 	(t_hit (*)(struct s_surface *, t_vec3, t_vec3))sphere_intersection,
	// 	NULL, COLOR3(1, 0, 0), VEC3(-50.0, -100.5, 1), 100};
	on_update(&mlx);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, on_close, &mlx);
	mlx_loop(mlx.ptr);
}
