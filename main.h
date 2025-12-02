#pragma once

#include "Libft/libft.h"
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define ERR_NO_FILE                                                            \
    "Usage Error: No input file provided. Please specify a scene file.\n"

#define ERR_FAILED_OPEN                                                        \
    "System Error: Could not open the specified file. Check path and "         \
    "permissions.\n"

#define ERR_FAILED_PARSER                                                      \
    "Fatal Error: Parsing aborted. See previous logs for details.\n"

#define MAX_SURFACES 15

typedef struct
{
    float x, y, z;
} t_vec3;
typedef t_vec3 t_color3;

typedef struct
{
    float t;
    t_vec3 normal;
} t_hit;

typedef struct
{
    t_hit (*hit_function)(void *, t_vec3, t_vec3);
    t_vec3 pos;
    t_vec3 normal;
    float diameter;
    float height;
    t_color3 color;
} t_cylinder;

typedef struct
{
    t_hit (*hit_function)(void *, t_vec3, t_vec3);
    t_vec3 pos;
    t_vec3 normal;
    t_color3 color;
} t_plane;

typedef struct
{
    t_hit (*hit_function)(void *, t_vec3, t_vec3);
    t_vec3 pos;
    float radius;
    t_color3 color;
} t_sphere;

typedef union {
    t_hit (*hit_function)(void *, t_vec3, t_vec3);
    t_cylinder *cylinder;
    t_sphere *sphere;
    t_plane *plane;
} t_surface;

struct s_raytracing
{
    struct
    {
        int w, h;
    } window;
    struct s_scene
    {
        struct s_camera
        {
            t_vec3 pos;
            t_vec3 fd;
            float fov;
        } camera;
        t_surface surfaces[MAX_SURFACES];
    } scene;
};

t_hit plane_intersection(t_plane *plane, t_vec3 org, t_vec3 dir);
t_hit sphere_intersection(t_sphere *sphere, t_vec3 org, t_vec3 dir);
t_hit cylinder_intersection(t_cylinder *cylinder, t_vec3 org, t_vec3 dir);

void on_close(char *message);
