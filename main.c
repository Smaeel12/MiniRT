/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:24:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/10/24 16:38:57 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#include <math.h>

#include "minilibx-linux/mlx.h"
#include "minilibx-linux/mlx_int.h"

int mlx_put_pixel_on_image(t_img *img, t_color3 color, int x, int y)
{
	char *dst;

	dst = img->data + (y * img->size_line + x * (img->abpp / 8));
	*(unsigned int *)dst = (int)(color.r * 255.999) << 16 |
						   (int)(color.g * 255.999) << 8 |
						   (int)(color.b * 255.999);
	return (0);
}

t_hit sphere_intersection(t_sphere *sphere, t_vec3 org, t_vec3 dir)
{
	t_vec3 oc;
	double a;
	double b;
	double disc;
	double t;

	oc	 = VSUB(sphere->pos, org);
	a	 = VDOT(dir, dir);
	b	 = -2.0 * VDOT(dir, oc);
	disc = b * b - 4 * a * (VDOT(oc, oc) - sphere->radius * sphere->radius);
	if (disc < 0.0)
		return ((t_hit){INFINITY, VEC3(0, 0, 0)});
	t = (-b - sqrt(disc)) / (2.0 * a);
	return ((t_hit){t, VNORM(VSUB(VADD(org, VMUL(t, dir)), sphere->pos))});
}

t_color3 per_pixel(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_hit hit;
	t_vec3 norm_rep;

	printf("%f %f %f\n", dir.x, dir.y, dir.z);
	hit = surface->hit_func(surface, org, dir);
	if (isinf(hit.t)) {
		t_vec3 unit_dir = VNORM(dir);
		double a		= 0.5 * (unit_dir.y + 1.0);
		return VADD(VMUL((1.0 - a), VEC3(1.0, 1.0, 1.0)),
					VMUL(a, VEC3(0.5, 0.7, 1.0)));
	}
	return (VMUL(0.5, VEC3(hit.n.x + 1, hit.n.y + 1, hit.n.z + 1)));
}

int on_update(t_mlx *mlx)
{
	t_surface *surface;
	t_vec3 color;
	t_vec3 dir;
	int y;
	int x;

	y = 0;
	while (y < mlx->h) {
		x = 0;
		while (x < mlx->w) {
			dir	  = VEC3(2 * ((x + 0.5) / (double)mlx->w) - 1,
						 1 - 2 * ((y + 0.5) / (double)mlx->h), -1.);
			color = per_pixel(mlx->scene.surface, mlx->scene.camera, dir);
			mlx_put_pixel_on_image(mlx->buffer, color, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->buffer, 0, 0);
	return (0);
}

int on_close(t_mlx *mlx)
{
	mlx_destroy_image(mlx->ptr, mlx->buffer);
	mlx_destroy_window(mlx->ptr, mlx->win);
	mlx_destroy_display(mlx->ptr);
	exit(0);
	return (0);
}

int main(void)
{
	t_mlx mlx;

	mlx.w			  = 800;
	mlx.h			  = 800;
	mlx.ptr			  = mlx_init();
	mlx.win			  = mlx_new_window(mlx.ptr, mlx.w, mlx.h, "3D");
	mlx.buffer		  = mlx_new_image(mlx.ptr, mlx.w, mlx.h);
	mlx.scene.camera  = VEC3(0, 0, -5.);
	mlx.scene.surface = (t_surface *)&(t_sphere){
		(t_hit (*)(struct s_surface *, t_vec3, t_vec3))sphere_intersection,
		NULL, COLOR3(1, 0, 0), VEC3(0, 0, -1), 0.5};
	mlx.scene.surface->next = (t_surface *)&(t_sphere){
		(t_hit (*)(struct s_surface *, t_vec3, t_vec3))sphere_intersection,
		NULL, COLOR3(1, 0, 0), VEC3(0, 0, -1), 0.5};

	on_update(&mlx);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, on_close, &mlx);
	mlx_loop(mlx.ptr);
}
