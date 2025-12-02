#pragma once

#include "main.h"

#define INF INFINITY

#define ERR_MISSING_FIELD "Syntax Error: A required field is missing.\n"
#define ERR_VALUE_OUT_RANGE                                                    \
    "Domain Error: The provided value is outside the allowable range.\n"
#define ERR_EXPECTED_FLOAT "Type Error: Expected a decimal number.\n"
#define ERR_EXPECTED_VECTOR                                                    \
    "Format Error: Expected a vector in format 'x,y,z' or 'r,g,b'.\n"
#define ERR_UNKNOWN_OBJECT "Syntax Error: Unknown object identifier.\n"
#define ERR_UNEXPECTED_FIELD                                                   \
    "Syntax Error: Unexpected extra field(s) provided for an object.\n"

enum e_field_type
{
    NONE,
    F_VEC3,
    F_VALUE,
};

typedef struct
{
    void *field;
    float min;
    float max;
    enum e_field_type type;
} t_field_rule;

typedef struct
{
    char *field_name;
    int nchar;
    void *object;
    t_field_rule *(*rules)(void *);
} t_parser_rule;

t_parser_rule *parser_rules(struct s_scene *scene);
int parse_objects(int file, t_parser_rule rules[]);

// #define FOV_RANGE "[0, 180]"
// #define NORM_RANGE "[0.0, 1.0]"
// #define INF_RANGE "[-infinity, infinity]"

static inline t_field_rule *camera_rules(struct s_camera *camera)
{
    static t_field_rule camera_rules[4];

    camera_rules[0] = (t_field_rule){&camera->pos, -INF, INF, F_VEC3};
    camera_rules[1] = (t_field_rule){&camera->fd, 0.0, 1.0, F_VEC3};
    camera_rules[2] = (t_field_rule){&camera->fov, 0, 180, F_VALUE};
    camera_rules[3] = (t_field_rule){NULL, 0, 0, NONE};
    return (camera_rules);
}

static inline t_field_rule *sphere_rules(t_sphere *sphere)
{
    static t_field_rule sphere_rules[4];

    sphere->hit_function =
        (t_hit (*)(void *, t_vec3, t_vec3))sphere_intersection;
    sphere_rules[0] = (t_field_rule){&sphere->pos, -INF, INF, F_VEC3};
    sphere_rules[1] = (t_field_rule){&sphere->radius, 0, INF, F_VALUE};
    sphere_rules[2] = (t_field_rule){&sphere->color, 0, 255, F_VEC3};
    sphere_rules[3] = (t_field_rule){NULL, 0, 0, NONE};
    return (sphere_rules);
}

static inline t_field_rule *plane_rules(t_plane *plane)
{
    static t_field_rule plane_rules[4];

    plane->hit_function = (t_hit (*)(void *, t_vec3, t_vec3))plane_intersection;
    plane_rules[0] = (t_field_rule){&plane->pos, -INF, INF, F_VEC3};
    plane_rules[1] = (t_field_rule){&plane->normal, 0.0, 1.0, F_VEC3};
    plane_rules[2] = (t_field_rule){&plane->color, 0, 255, F_VEC3};
    plane_rules[3] = (t_field_rule){NULL, 0, 0, NONE};
    return (plane_rules);
}

static inline t_field_rule *cylinder_rules(t_cylinder *cylinder)
{
    static t_field_rule *cylinder_rules;

    cylinder->hit_function =
        (t_hit (*)(void *, t_vec3, t_vec3))cylinder_intersection;
    cylinder_rules[0] = (t_field_rule){&cylinder->pos, -INF, INF, F_VEC3};
    cylinder_rules[1] = (t_field_rule){&cylinder->normal, 0.0, 1.0, F_VEC3};
    cylinder_rules[2] = (t_field_rule){&cylinder->diameter, 0, INF, F_VALUE};
    cylinder_rules[3] = (t_field_rule){&cylinder->height, 0, INF, F_VALUE};
    cylinder_rules[4] = (t_field_rule){&cylinder->color, 0, 255, F_VEC3};
    cylinder_rules[5] = (t_field_rule){NULL, 0, 0, NONE};
    return (cylinder_rules);
}

static inline int parse_vector(t_field_rule rule, char *vector)
{
    float *input;
    char *value;
    int i;

    i = 0;
    input = rule.field;
    while (i < 3)
    {
        value = ft_strsep(&vector, ',');
        if (!*value)
            return (on_close(ERR_EXPECTED_VECTOR), -1);
        input[i] = atof(value);
        if (input[i] > rule.max || input[i] < rule.min)
            return (on_close(ERR_VALUE_OUT_RANGE), -1);
        i++;
    }
    return (0);
}

static inline int parse_value(t_field_rule rule, char *float_value)
{
    float *input;
    int i;

    i = 0;
    input = rule.field;
    while (float_value[i] &&
           ((float_value[i] <= '9' && float_value[i] >= '0') ||
            float_value[i] == '.'))
        i++;
    if (float_value[i])
        return (on_close(ERR_EXPECTED_FLOAT), -1);
    *input = atof(float_value); // atof needs implement
    if (*input > rule.max || *input < rule.min)
        return (on_close(ERR_VALUE_OUT_RANGE), -1);
    return (0);
}

static inline int parse_fields(char *tokens, t_field_rule rules[])
{
    char *value;
    int i;

    i = 0;
    while (true)
    {
        value = ft_strsep(&tokens, ' ');
        if (!rules[i].field && !value)
            break;
        if (!value)
            return (on_close(ERR_MISSING_FIELD), -1);
        if (!rules[i].field)
            return (on_close(ERR_UNEXPECTED_FIELD), -1);
        if (rules[i].type == F_VEC3 && parse_vector(rules[i], value))
            return (-1);
        else if (rules[i].type == F_VALUE && parse_value(rules[i], value))
            return (-1);
        i++;
    }
    return (0);
}
