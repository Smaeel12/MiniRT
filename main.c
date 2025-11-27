/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 01:34:30 by iboubkri          #+#    #+#             */
/*   Updated: 2025/11/27 17:09:00 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#define VADD(a, b) ((t_vec3){{a.x + b.x, a.y + b.y, a.z + b.z}})
#define VSUB(a, b) ((t_vec3){{a.x - b.x, a.y - b.y, a.z - b.z}})
#define VDOT(a, b) (a.x * b.x + a.y * b.y + a.z * b.z)
#define VMUL(n, vec) ((t_vec3){{n * vec.x, n * vec.y, n * vec.z}})

#define VNORM vnorm
t_vec3 vnorm(t_vec3 vec)
{
	double len;

	len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return ((t_vec3){{vec.x / len, vec.y / len, vec.z / len}});
}
t_hit plane_intersection(t_plane *plane, t_vec3 org, t_vec3 dir)
{
	printf("POS: %f %f %f\n", plane->pos.x, plane->pos.y, plane->pos.z);
	printf("NORMAL: %f %f %f\n", plane->normal.x, plane->normal.y,
		   plane->normal.z);
	printf("COLOR : %f %f %f\n", plane->color.x, plane->color.y,
		   plane->color.z);
	return ((t_hit){VEC3(0, 0, 0), 1});
}
t_hit cylinder_intersection(t_cylinder *cylinder, t_vec3 org, t_vec3 dir)
{
	printf("POS: %f %f %f\n", cylinder->pos.x, cylinder->pos.y,
		   cylinder->pos.z);
	printf("NORMAL: %f %f %f\n", cylinder->normal.x, cylinder->normal.y,
		   cylinder->normal.z);
	printf("COLOR : %f %f %f\n", cylinder->color.x, cylinder->color.y,
		   cylinder->color.z);
	printf("DIAMETER: %f HEIGHT: %f\n", cylinder->diameter, cylinder->height);
	return ((t_hit){VEC3(0, 0, 0), 1});
}
t_hit sphere_intersection(t_sphere *sphere, t_vec3 org, t_vec3 dir)
{
	// t_vec3 oc;
	// double a;
	// double b;
	// double disc;
	// double t;
	// oc		= VSUB(sphere->pos, org);
	// a		= VDOT(dir, dir);
	// b		= -2.0 * VDOT(dir, oc);
	// disc = b * b - 4 * a * (VDOT(oc, oc) - sphere->diameter *
	// sphere->diameter); if (disc < 0.0) 	return ((t_hit){VEC3(-1, -1, -1),
	// INFINITY}); t = (-b - sqrt(disc)) / (2.0 * a); return (//
	// ((t_hit){VNORM(VSUB(VADD(org, VMUL(t, dir)), sphere->pos)), t}));
	printf("POS: %f %f %f\n", sphere->pos.x, sphere->pos.y, sphere->pos.z);
	printf("COLOR : %f %f %f\n", sphere->color.x, sphere->color.y,
		   sphere->color.z);
	printf("DIAMETER: %f\n", sphere->diameter);
	return ((t_hit){VEC3(0, 0, 0), 1});
}

int mlx_put_pixel_on_image(t_img *img, t_color3 color, int x, int y)
{
	char *dst;

	dst = img->data + (y * img->size_line + x * (img->abpp / 8));
	*(unsigned int *)dst = (int)(color.r * 255.999) << 16 |
						   (int)(color.g * 255.999) << 8 |
						   (int)(color.b * 255.999);
	return (0);
}

int on_update(struct s_app *rt)
{
	t_hit hit;
	t_vec3 dir;
	int x;
	int y;

	y = 0;
	while (y < rt->window.h) {
		x = 0;
		while (x < rt->window.w) {
			dir = VEC3(2 * ((x + 0.5) / (float)rt->window.w) - 1,
					   1 - 2 * ((y + 0.5) / (float)rt->window.h), -1);
			hit = rt->scene.surfaces->hit_function(rt->scene.surfaces,
												   rt->scene.camera.pos, dir);
			if (!isinf(hit.t)) {
				mlx_put_pixel_on_image(rt->window.framebuffer,
									   VMUL(0.5, VADD(VEC3(1, 1, 1), hit.n)), x,
									   y);
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt->window.mlx, rt->window.win,
							rt->window.framebuffer, 0, 0);
	return (0);
}

int on_close(struct s_app *rt)
{
	mlx_destroy_image(rt->window.mlx, rt->window.framebuffer);
	mlx_destroy_window(rt->window.mlx, rt->window.win);
	mlx_destroy_display(rt->window.mlx);
	return (exit(0), 0);
}

int main(void)
{
	struct s_app rt;

	memset(&rt, 0, sizeof(rt));

	rt.window.h = 800;
	rt.window.w = 800;

	// rt.window.mlx = mlx_init();
	// rt.window.win =
	// 	mlx_new_window(rt.window.mlx, rt.window.w, rt.window.h, "3D");

	// rt.window.framebuffer =
	// 	mlx_new_image(rt.window.mlx, rt.window.w, rt.window.h);

	int fd;
	fd = open("objs.rt", O_RDONLY);
	if (fd < 0)
		return (dprintf(2, "CAN'T READ FILE"), -1);
	rt.scene.surfaces = malloc(MAX_SURFACES * sizeof(t_surface));
	memset(rt.scene.surfaces, 0, MAX_SURFACES * sizeof(t_surface));

	init_scene(&rt, fd);
	printf("POS: %f %f %f\n", rt.scene.camera.pos.x, rt.scene.camera.pos.y,
		   rt.scene.camera.pos.z);
	printf("FD : %f %f %f\n", rt.scene.camera.fd.x, rt.scene.camera.fd.y,
		   rt.scene.camera.fd.z);
	printf("FOV: %d\n", rt.scene.camera.fov);

	for (int i = 0; i < MAX_SURFACES; i++) {
		printf("=======\n");
		if (!rt.scene.surfaces[i].hit_function)
			break;
		rt.scene.surfaces[i].hit_function(&rt.scene.surfaces[i], VEC3(0, 0, 0),
										  VEC3(0, 0, 0));
	}
	free(rt.scene.surfaces);

	// on_update(&rt);

	// mlx_hook(rt.window.win, DestroyNotify, StructureNotifyMask, on_close,
	// &rt); mlx_loop(rt.window.mlx);
}