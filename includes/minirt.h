#ifndef MINIRT_H
# define MINIRT_H

# include "mlx.h"
# include "libft.h"
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <fcntl.h>


# define KEY_ESC 65307
# define WIN_W 2000
# define WIN_H 1500

typedef struct s_app
{
	void	*mlx;
	void	*win;
	void	*img;
	char 	*addr;
	int		bpp;
	int		ll;
	int		endian;
	int		w;
	int		h;
}			t_app;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_ambient
{
	int	has;
	double	ratio;
	t_color	color;
}	t_ambient;

typedef struct s_scene
{
	t_ambient	a;
}	t_scene;

int	init_app(t_app *app, int w, int h, char *title);
int	destroy_app(t_app *app);
int	on_key(int keycode, t_app *app);
int	on_close(t_app *app);

//func for drawing
void	fill_black(t_app *app);
void    put_frame(t_app *app);
unsigned int	rgb(int r, int g, int b);
void	put_pixel(t_app *app, int x, int y, unsigned int color);
void	clear_img(t_app *app);
void	render_test(t_app *app);
int		loop_frame(void *param);

/* small parsing helpers */
int  parse_double(const char *s, double *out);
int  parse_int(const char *s, int *out);
int  parse_color(const char *s, t_color *c);
int  in_range_d(double v, double mn, double mx);
int  in_range_i(int v, int mn, int mx);

/* ambient scene loading (A only for today) */
int  load_scene(const char *path, t_scene *sc);
void free_scene(t_scene *sc);

#endif
