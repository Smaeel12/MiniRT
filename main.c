#include "main.h"
#include "parser_utils.h"

#define VMUL(n, vec)                                                           \
    (t_vec3)                                                                   \
    {                                                                          \
        n *vec.x, n *vec.y, n *vec.z                                           \
    }
#define VADD(a, b)                                                             \
    (t_vec3)                                                                   \
    {                                                                          \
        a.x + b.x, a.y + b.y, a.z + b.z                                        \
    }

void on_close(char *message)
{
    write(2, message, ft_strlen(message));
    exit(-1);
}

void write_color(t_color3 color)
{
    printf("%d %d %d\n", (int)color.x, (int)color.y, (int)color.z);
}

int on_render(struct s_raytracing *rt)
{
    t_hit prev_hit;
    t_vec3 dir;
    t_hit hit;
    int idx;
    int x;
    int y;

    y = 0;
    hit = (t_hit){INF, (t_vec3){0, 0, 0}};
    prev_hit = (t_hit){INF, (t_vec3){0, 0, 0}};
    while (y < rt->window.h)
    {
        x = 0;
        while (x < rt->window.w)
        {
            idx = 0;
            dir = (t_vec3){2.0 * x / (float)rt->window.w - 1.0,
                           1.0 - 2.0 * y / (float)rt->window.h, -1};
            while (idx < MAX_SURFACES && rt->scene.surfaces[idx].hit_function)
            {
                hit = rt->scene.surfaces[idx].hit_function(
                    &rt->scene.surfaces[idx], rt->scene.camera.pos, dir);
                if (hit.t < prev_hit.t)
                    hit = prev_hit;
                idx++;
            }
            if (hit.t != INF)
                write_color((t_color3){255, 255, 255});
            else
            {
                float a = 0.5 * (dir.y + 1.0);
                write_color(
                    VMUL(255, VADD(VMUL((1 - a), ((t_color3){1.0, 1.0, 1.0})),
                                   VMUL(a, ((t_color3){0.5, 0.7, 1.0})))));
            }
            x++;
        }
        y++;
    }
    return (0);
}

int main(int ac, char **av)
{
    struct s_raytracing rt;
    int fd;

    if (ac < 2)
        on_close(ERR_NO_FILE);
    fd = open(av[1], O_RDONLY);
    if (fd < 0)
        on_close(ERR_FAILED_OPEN);
    ft_memset(&rt, 0, sizeof(struct s_raytracing));
    rt.window.h = 256;
    rt.window.w = 256;
    ft_memset(rt.scene.surfaces, 0, MAX_SURFACES * sizeof(t_surface));
    // printf("P3\n%i %i\n255\n", rt.window.h, rt.window.w);
    parse_objects(fd, parser_rules(&rt.scene));

    // printf("CAMERA: %f %f %f\t%f %f %f\t%f\n", rt.scene.camera.pos.x,
    //        rt.scene.camera.pos.y, rt.scene.camera.pos.z,
    //        rt.scene.camera.fd.x, rt.scene.camera.fd.y, rt.scene.camera.fd.z,
    //        rt.scene.camera.fov);

    for (int i = 0; rt.scene.surfaces[i].hit_function; i++)
    {
        printf("SURFACE: %p\n%f %f %f\n%f\n%f %f %f\n",
               rt.scene.surfaces[i].hit_function,
               rt.scene.surfaces[i].sphere->pos.x,
               rt.scene.surfaces[i].sphere->pos.y,
               rt.scene.surfaces[i].sphere->pos.z,
               rt.scene.surfaces[i].sphere->radius,
               rt.scene.surfaces[i].sphere->color.x,
               rt.scene.surfaces[i].sphere->color.y,
               rt.scene.surfaces[i].sphere->color.z);
    }
    // on_render(&rt);
    return (0);
}
