/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 01:34:30 by iboubkri          #+#    #+#             */
/*   Updated: 2025/11/20 12:50:00 by iboubkri         ###   ########.fr       */
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
	disc = b * b - 4 * a * (VDOT(oc, oc) - sphere->diameter * sphere->diameter);
	if (disc < 0.0)
		return ((t_hit){VEC3(-1, -1, -1), INFINITY});
	t = (-b - sqrt(disc)) / (2.0 * a);
	return ((t_hit){VNORM(VSUB(VADD(org, VMUL(t, dir)), sphere->pos)), t});
}

#define MAX_SURFACES 10

t_vec3 str_to_vec(char *line)
{
}

static int camera_parse(struct s_camera *camera, char *pos, char *fd, char *fov)
{
	t_vec3 up = VEC3(0, 1, 0);
	if (!pos || !fd || !fov)
		return -1;

	camera->pos = (t_vec3){atof(strtok(pos, ",")), atof(strtok(pos, ",")),
						   atof(strtok(pos, ","))};
	camera->fd	= (t_vec3){atof(strtok(fd, ",")), atof(strtok(fd, ",")),
						   atof(strtok(fd, ","))};
	camera->fov = atoi(strtok(fov, ""));

	if (!(camera->fov.x == 0 && camera->fov.y == 0 && camera->fov.z == 0))
		up = VEC3(0, 0, 1);
	camera->rt = VCROSS(camera->up, camera.fd);
	camera->up = VCROSS(camera->rt, camera.fd);

	return 0;
}

int init_scene(struct s_app *rt)
{
	int fd;
	int idx;
	char *line;
	char *id;

	fd				   = open("objs.rt", O_RDONLY);
	rt->scene.surfaces = malloc(MAX_SURFACES * sizeof(t_surface));
	memset(rt->scene.surfaces, 0, MAX_SURFACES * sizeof(t_surface));
	while ((line = get_next_line(fd))) {
		id = strtok(line, " \t\v\r");
		if (strncmp(id, "C", 2)) {
			camera_parse(rt->scene, strtok(line, " \t\v\r"),
						 strtok(line, " \t\v\r"), strtok(line, " \t\v\r"));
		}
		if (strncmp(id, "pl", 3)) {
			rt->scene.surfaces[idx++] = (t_surface)plane_parse(
				rt->scene, strtok(line, " \t\v\r"), strtok(line, " \t\v\r"),
				strtok(line, " \t\v\r"));
		}
		if (strncmp(id, "sp", 3)) {
			rt->scene.surfaces[idx++] = (t_surface)sphere_parse(
				rt->scene, strtok(line, " \t\v\r"), strtok(line, " \t\v\r"),
				strtok(line, " \t\v\r"));
		}
		if (strncmp(id, "cy", 3)) {
			rt->scene.surfaces[idx++] = (t_surface)cylinder_parse(
				rt->scene, strtok(line, " \t\v\r"), strtok(line, " \t\v\r"),
				strtok(line, " \t\v\r"), strtok(line, " \t\v\r"),
				strtok(line, " \t\v\r"));
		}
	}
	return (0);
}

int init_scene(struct s_app *rt)
{
	char *line;
	char *id;

	int fd = open("objs.rt", O_RDONLY);
	while (true) {
		line = get_next_line(fd);
		if (!line)
			break;
		id = strtok(line, " ,");
	}
	return 0;
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

	rt.window.h = 800;
	rt.window.w = 800;

	rt.window.mlx = mlx_init();
	rt.window.win =
		mlx_new_window(rt.window.mlx, rt.window.w, rt.window.h, "3D");

	rt.window.framebuffer =
		mlx_new_image(rt.window.mlx, rt.window.w, rt.window.h);

	init_scene(&rt);
	printf("%f\n", rt.scene.surfaces->sphere.diameter);
	on_update(&rt);

	mlx_hook(rt.window.win, DestroyNotify, StructureNotifyMask, on_close, &rt);
	mlx_loop(rt.window.mlx);
}