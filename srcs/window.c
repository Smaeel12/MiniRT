#include "minirt.h"

static void	setup_hooks(t_app *app)
{
	mlx_hook(app->win, 2, 1L << 0, on_key, app);
	mlx_hook(app->win, 17, 0, on_close, app);
}

int	init_app(t_app *app, int w, int h, char *title)
{
	app->w = w;
	app->h = h;
	app->mlx = mlx_init();
	if (!app->mlx)
		return (0);
	app->win = mlx_new_window(app->mlx, app->w, app->h, title);
	if (!app->win)
		return (destroy_app(app), 0);
	app->img = mlx_new_image(app->mlx, app->w, app->h);
	if (!app->img)
		return (destroy_app(app), 0);
	setup_hooks(app);
	return (1);
}

int	destroy_app(t_app *app)
{
	if (app->img)
	{
		mlx_destroy_image(app->mlx, app->img);
		app->img = NULL;
	}
	if (app->win)
	{
		mlx_destroy_window(app->mlx, app->win);
		app->win = NULL;
	}
	if (app->mlx)
	{
		mlx_destroy_display(app->mlx);
		free(app->mlx);
		app->mlx = NULL;
	}
	return (0);
}
void	fill_black(t_app *app)
{
	int bytes;
	int total;

	bytes = app->bpp / 8;
	total = app->ll * app->h;
	(void)bytes;
	memset(app->addr, 0, total);
}

void	put_frame(t_app *app)
{
	mlx_put_image_to_window(app->mlx, app->win, app->img, 0, 0);
}