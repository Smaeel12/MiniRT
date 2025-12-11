#include "includes/parser.h"

void fallback_message(char *message)
{
	write(2, message, ft_strlen(message));
}

int on_close(struct s_raytracer *rt)
{
	if (!rt)
		return (exit(0), 1);

	printf("P3\n%d %d\n255\n", rt->win.width, rt->win.height);
	for (int y = 0; y < rt->win.height; y++) {
		for (int x = 0; x < rt->win.width; x++) {
			int idx = y * rt->win.width + x;
			t_vec3 color =
				VMUL(255.99, VMUL(1.0 / rt->nsamples, rt->acumm[idx]));
			printf("%d %d %d\n", (int)color.x, (int)color.y, (int)color.z);
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

float rand_float()
{
	return 0.5 * (1 - 2 * rand() / (RAND_MAX + 1.0));
}

#define MAX_SAMPLES 10

#define WRITE_COLOR(vec) ((int)vec.x << 16 | (int)vec.y << 8 | (int)vec.z)

int mlx_pixel_put_to_image(t_img *image, int x, int y, t_color3 color)
{
	char *dst;

	dst = image->data + y * image->size_line + x * image->abpp / 8;
	*(unsigned int *)dst =
		((int)color.x << 16) | ((int)color.y << 8) | ((int)color.z << 0);
	return 0;
}

#include <time.h>

int on_render(struct s_raytracer *rt)
{
	t_vec3 dir;
	t_hit hit;
	int x;
	int y;

	clock_t current_time = clock();

	float aratio = (float)rt->win.width / (float)rt->win.height;

	t_vec3 start_pixel =

		y = 0;
	while (y < rt->win.height) {
		x = 0;
		while (x < rt->win.width) {
			dir = (t_vec3){
				(2.0 * (x + rand_float()) / (float)rt->win.width - 1.0) *
					rt->scene.camera.fov * aratio,	// x
				(1.0 - 2.0 * (y + rand_float()) / (float)rt->win.height) *
					rt->scene.camera.fov,  // y
				-1,						   // z
			};

			dir = VADD(VADD(VMUL(dir.x, rt->scene.camera.rt),
							VMUL(dir.y, rt->scene.camera.up)),
					   VMUL(dir.z, rt->scene.camera.fd));

			dprintf(2, "%f %f %f\n", dir.x, dir.y, dir.z);
			hit = (t_hit){(t_vec3){0, 0, 0}, INF};
			if (rt->scene.surfaces->hit_function)
				hit = rt->scene.surfaces->hit_function(
					rt->scene.surfaces, rt->scene.camera.pos, dir);
			if (hit.t != INF) {
				rt->acumm[y * rt->win.width + x] =
					VADD(rt->acumm[y * rt->win.width + x],
						 VMUL(0.5, VADD(((t_vec3){1, 1, 1}), hit.normal)));
			}
			else {
				float a = 0.5 * (dir.y + 1.0);
				rt->acumm[y * rt->win.width + x] =
					VADD(rt->acumm[y * rt->win.width + x],
						 VADD(VMUL(1 - a, ((t_color3){1.0, 1.0, 1.0})),
							  VMUL(a, ((t_color3){0.5, 0.7, 1.0}))));
			}
			mlx_pixel_put_to_image(
				rt->win.frame, x, y,
				VMUL(255, VMUL(1.0 / rt->nsamples,
							   rt->acumm[y * rt->win.width + x])));
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
#define FOCAL_LENGHT 1.0

int main(int ac, char **av)
{
	struct s_raytracer rt;
	int fd;

	if (ac < 2)
		return (fallback_message(ERR_NO_FILE), -1);

	ft_bzero(&rt, sizeof(struct s_raytracer));

	rt.scene.surfaces = malloc(MAX_SURFACES * sizeof(t_surface));
	ft_bzero(rt.scene.surfaces, MAX_SURFACES * sizeof(t_surface));
	parse_objects(open(av[1], O_RDWR), parser_rules(&rt.scene));

	rt.win.width		= 1080;
	rt.win.height		= rt.win.width * ASPECT_RATIO;
	rt.scene.camera.fov = tan(rt.scene.camera.fov * 0.5 * M_PI / 180.0);

	rt.scene.camera.viewport.delta_u = (t_vec3){
		2.0 * rt.scene.camera.fov * FOCAL_LENGHT * rt.win.height, 0, 0};

	rt.scene.camera.viewport.delta_v = (t_vec3){
		0, -(2.0 * rt.scene.camera.fov * FOCAL_LENGHT) / rt.win.height, 0};

	rt.scene.camera.viewport.px00loc = VSUB(
		rt.scene.camera.pos,
		((t_vec3){
			rt.win.height * rt.scene.camera.fov * FOCAL_LENGHT / rt.win.width,
			rt.scene.camera.fov * FOCAL_LENGHT,
			FOCAL_LENGHT,
		}));

	rt.scene.camera.rt = VNORM(VCROSS(rt.scene.camera.fd, ((t_vec3){0, 1, 0})));
	rt.scene.camera.up = VNORM(VCROSS(rt.scene.camera.rt, rt.scene.camera.fd));

	rt.mlx		 = mlx_init();
	rt.win.ptr	 = mlx_new_window(rt.mlx, rt.win.width, rt.win.height, "3DRT");
	rt.win.frame = mlx_new_image(rt.mlx, rt.win.width, rt.win.height);

	rt.acumm = malloc(rt.win.height * rt.win.width * sizeof(t_vec3));
	ft_bzero(rt.acumm, rt.win.height * rt.win.width * sizeof(t_vec3));

	rt.nsamples = 1;

	mlx_hook(rt.win.ptr, DestroyNotify, StructureNotifyMask,
			 (int (*)(void *))on_close, &rt);
	// mlx_loop_hook(rt.mlx, on_render, &rt);
	on_render(&rt);
	mlx_loop(rt.mlx);
	return (0);
}
