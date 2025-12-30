/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 04:44:53 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/30 21:31:00 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/mlx_utils.h"
#include "includes/parser.h"

#define ASPECT_RATIO 1.77777777778
#define ERR_INV_EXT "System Error: Invalid extension the file should end .rt"

int	keypress(int keycode, struct s_raytracer *rt)
{
	on_close(rt);
	return (0);
}

void	fallback_message(char *message)
{
	write(2, message, ft_strlen(message));
}

int	init_camera(struct s_camera *camera, int width, int height)
{
	float	ap;
	float	theta;
	float	vh;
	float	vw;

	ap = (float)width / (float)height;
	vw = tan(0.5 * (camera->fov * M_PI / 180.0));
	vh = vw / ap;
	camera->w = VMUL(-1.0, camera->w);
	camera->v = (t_vec3){0, 1, 0};
	if (fabs(camera->w.y) > 0.999)
		camera->v = (t_vec3){0, 0, 1};
	camera->u = VNORM(VCROSS(camera->v, camera->w));
	camera->v = VNORM(VCROSS(camera->w, camera->u));
	camera->vp.du = VMUL(2.0 * vw, camera->u);
	camera->vp.dv = VMUL(2.0 * -vh, camera->v);
	camera->vp.px00loc = VSUB(VSUB(VSUB(camera->pos, camera->w), VMUL(0.5,
					camera->vp.du)), VMUL(0.5, camera->vp.dv));
	camera->vp.du = VMUL(1.0 / width, camera->vp.du);
	camera->vp.dv = VMUL(1.0 / height, camera->vp.dv);
	camera->vp.px00loc = VADD(camera->vp.px00loc, VMUL(0.5, VADD(camera->vp.du,
					camera->vp.dv)));
	return (0);
}

int	check_file(char *filename)
{
	char	*ext;
	int		fd;

	ext = ft_strchr(filename, '.');
	if (!ext || ft_strncmp(".rt", ext, 4))
		return (fallback_message(ERR_INV_EXT), -1);
	fd = open(filename, O_RDWR);
	if (fd == -1)
		return (fallback_message(ERR_FAILED_OPEN), -1);
	return (fd);
}

int	main(int ac, char **av)
{
	struct s_raytracer	rt;
	int					fd;

	if (ac < 2)
		return (fallback_message(ERR_NO_FILE), -1);
	ft_bzero(&rt, sizeof(struct s_raytracer));
	rt.win.width = 1080;
	rt.win.height = rt.win.width / ASPECT_RATIO;
	fd = check_file(av[1]);
	if (fd < 0 || parse_file(fd, parser_rules(&rt.scene)))
		return (1);
	init_camera(&rt.scene.camera, rt.win.width, rt.win.height);
	rt.mlx = mlx_init();
	rt.win.ptr = mlx_new_window(rt.mlx, rt.win.width, rt.win.height, "3DRT");
	rt.win.frame = mlx_new_image(rt.mlx, rt.win.width, rt.win.height);
	rt.acumm = malloc(rt.win.height * rt.win.width * sizeof(t_vec3));
	ft_bzero(rt.acumm, rt.win.height * rt.win.width * sizeof(t_vec3));
	rt.nsamples = 1;
	mlx_hook(rt.win.ptr, KeyPress, KeyPressMask, (t_hook2)keypress, &rt);
	mlx_hook(rt.win.ptr, DestroyNotify, 1L << 17, (t_hook)on_close, &rt);
	mlx_loop_hook(rt.mlx, on_render, &rt);
	return (mlx_loop(rt.mlx), 0);
}
