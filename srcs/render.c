#include "minirt.h"

unsigned int rgb(int r, int g, int b)
{
    if (r < 0)
        r = 0;
    if (r > 255)
        r = 255;
    if (g < 0)
        g = 0;
    if (g > 255)
        g = 255;
    if (b < 0)
        b = 0;
    if (b > 255)
        b = 255;
    return ((unsigned int)((r << 16) | (g << 8) | b));
}
void put_pixel(t_app *app, int x, int y, unsigned int color)
{
    char *p;
    int bytes;

    if (!app || !app->addr)
        return;
    if (x < 0 || y < 0 || x >= app->w || y >= app->h)
        return;
    bytes = app->bpp / 8;
    p = app->addr + (y * app->ll + x * bytes);
    *(unsigned int *)p = color;
}
void clear_img(t_app *app)
{
    int y = 0;

    if (!app || !app->addr)
        return;
    while (y < app->h)
    {
        ft_memset(app->addr + y * app->ll, 0, (size_t)app->ll);
        y++;
    }
}
void render_test(t_app *app)
{
    int x;
    int y;
    unsigned int c;

    y = 0;
    while (y < app->h)
    {
        x = 0;
        while (x < app->w)
        {
            c = rgb((x * 255) / (app->w - 1), (y * 255) / (app->h - 1), 32);
            if (x % 50 == 0 || y % 50 == 0)
                c = rgb(255, 255, 255);
            put_pixel(app, x, y, c);
            x++;
        }
        y++;
    }
}
int loop_frame(void *param)
{
    t_app *app = (t_app *)param;
    // clear_img(app);
    render_test(app);
    put_frame(app);
    return (0);
}
