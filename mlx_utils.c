/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:42:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/30 23:52:53 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/mlx_utils.h"

int	mlx_pixel_put_to_image(t_img *image, int x, int y, t_color3 color)
{
	char	*dst;

	dst = image->data + y * image->size_line + x * image->bpp / 8;
	color = (t_color3){CLAMP(sqrt(color.x)), CLAMP(sqrt(color.y)),
		CLAMP(sqrt(color.z))};
	color = VMUL(255.99, color);
	*(unsigned int *)dst = (int)(color.x) << 16 | (int)(color.y) << 8
		| (int)(color.z);
	return (0);
}

int	on_close(struct s_raytracer *rt)
{
	int		idx;
	t_vec3	color;

	if (!rt)
		return (exit(0), 1);
	if (rt->win.frame)
		mlx_destroy_image(rt->mlx, rt->win.frame);
	if (rt->win.ptr)
		mlx_destroy_window(rt->mlx, rt->win.ptr);
	mlx_destroy_display(rt->mlx);
	free(rt->acumm);
	free(rt->mlx);
	exit(0);
	return (1);
}

t_color3	per_pixel(struct s_scene *scene, t_vec3 org, t_vec3 dir)
{
	t_color3	color;
	t_hit		hit;
	t_vec3		a;
	t_vec3		l;
	t_vec3		d;

	if (!scene->surfaces->hit_function)
		return ((t_color3){0});
	hit = scene->surfaces->hit_function(scene->surfaces, org, dir);
	if (hit.t == INF)
		return ((t_color3){0});
	color = VMUL(1 / 255.0, hit.surface->color);
	l = VSUB(scene->light.pos, hit.p);
	a = HADAMARD_PRODUCT(VMUL(1.0f / 255.0f, scene->ambient.color), color);
	a = VMUL(scene->ambient.ratio, a);
	if (scene->surfaces->hit_function(scene->surfaces, VADD(hit.p, VMUL(EPS,
					hit.normal)), VNORM(l)).t < VLENGHT(l))
		return (a);
	d = HADAMARD_PRODUCT(VMUL(1 / 255.0, scene->light.color), color);
	d = VMUL(scene->light.ratio * CLAMP(VDOT(hit.normal, VNORM(l))), d);
	return (VADD(d, a));
}

int	update_window(struct s_raytracer *rt)
{
	char	name[100];
	char	*nsamples;
	int		i;
	int		j;

	i = 0;
	j = 0;
	nsamples = ft_itoa(rt->nsamples++);
	while (nsamples[j] && i < 100)
		name[i++] = nsamples[j++];
	j = 0;
	while (" samples"[j] && i < 100)
		name[i++] = " samples"[j++];
	name[i] = '\0';
	XStoreName(rt->mlx->display, rt->win.ptr->window, name);
	mlx_put_image_to_window(rt->mlx, rt->win.ptr, rt->win.frame, 0, 0);
	return (free(nsamples), 0);
}

int	on_render(struct s_raytracer *rt)
{
	float	aratio;
	t_vec3	dir;
	int		x;
	int		y;

	y = -1;
	aratio = (float)rt->win.width / (float)rt->win.height;
	while (++y < rt->win.height)
	{
		x = -1;
		while (++x < rt->win.width)
		{
			dir = VMUL(x + RANF(-0.5, 0.5), rt->scene.camera.vp.du);
			dir = VADD(dir, VMUL(y + RANF(-0.5, 0.5), rt->scene.camera.vp.dv));
			dir = VADD(dir, rt->scene.camera.vp.px00loc);
			dir = VNORM(VSUB(dir, rt->scene.camera.pos));
			rt->acumm[y * rt->win.width + x] = VADD(rt->acumm[y * rt->win.width
					+ x], per_pixel(&rt->scene, rt->scene.camera.pos, dir));
			mlx_pixel_put_to_image(rt->win.frame, x, y, VMUL(1.0f
					/ rt->nsamples, rt->acumm[y * rt->win.width + x]));
		}
	}
	return (update_window(rt));
}
