#include "minirt.h"

int	main(void)
{
	t_app	app;

	if (!init_app(&app, WIN_W, WIN_H, "miniRT"))
		return (1);
    fill_black(&app);
    put_frame(&app);
	mlx_loop(app.mlx);
	return (0);
}
