/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaloulid <jaloulid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:12:24 by jaloulid          #+#    #+#             */
/*   Updated: 2025/10/20 14:12:26 by jaloulid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(void)
{
	t_app	app;

	if (!init_app(&app, WIN_W, WIN_H, "miniRT"))
		return (1);
    fill_black(&app);
    put_frame(&app);
	mlx_loop_hook(app.mlx, loop_frame, &app);
	mlx_loop(app.mlx);
	return (0);
}
