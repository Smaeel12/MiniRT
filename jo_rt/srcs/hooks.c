#include "minirt.h"

int	on_key(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		on_close(app);
	return (0);
}

int	on_close(t_app *app)
{
	destroy_app(app);
	exit(0);
	return (0);
}
