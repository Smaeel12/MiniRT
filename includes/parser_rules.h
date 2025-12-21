#pragma once

#include "structs.h"

static inline t_field_rule *camera_rules(struct s_camera *camera)
{
	static t_field_rule camera_rules[4];

	camera_rules[0] = (t_field_rule){&camera->pos, -INF, INF, F_VEC3};
	camera_rules[1] = (t_field_rule){&camera->w, -1.0, 1.0, F_VEC3};
	camera_rules[2] = (t_field_rule){&camera->fov, 0, 180, F_VALUE};
	camera_rules[3] = (t_field_rule){NULL, 0, 0, NONE};
	return (camera_rules);
}

static inline t_field_rule *sphere_rules(t_sphere *sphere)
{
	static t_field_rule sphere_rules[4];

	sphere->hit_function = sphere_intersection;
	sphere_rules[0]		 = (t_field_rule){&sphere->pos, -INF, INF, F_VEC3};
	sphere_rules[1]		 = (t_field_rule){&sphere->diameter, 0, INF, F_VALUE};
	sphere_rules[2]		 = (t_field_rule){&sphere->color, 0, 255, F_VEC3};
	sphere_rules[3]		 = (t_field_rule){NULL, 0, 0, NONE};
	return (sphere_rules);
}

static inline t_field_rule *plane_rules(t_plane *plane)
{
	static t_field_rule plane_rules[4];

	plane->hit_function = plane_intersection;
	plane_rules[0]		= (t_field_rule){&plane->pos, -INF, INF, F_VEC3};
	plane_rules[1]		= (t_field_rule){&plane->normal, -1.0, 1.0, F_VEC3};
	plane_rules[2]		= (t_field_rule){&plane->color, 0, 255, F_VEC3};
	plane_rules[3]		= (t_field_rule){NULL, 0, 0, NONE};
	return (plane_rules);
}

static inline t_field_rule *cylinder_rules(t_cylinder *cylinder)
{
	static t_field_rule cylinder_rules[6];

	cylinder->hit_function = cylinder_intersection;
	cylinder_rules[0]	   = (t_field_rule){&cylinder->pos, -INF, INF, F_VEC3};
	cylinder_rules[1]	   = (t_field_rule){&cylinder->axis, -1.0, 1.0, F_VEC3};
	cylinder_rules[2] = (t_field_rule){&cylinder->diameter, 0, INF, F_VALUE};
	cylinder_rules[3] = (t_field_rule){&cylinder->height, 0, INF, F_VALUE};
	cylinder_rules[4] = (t_field_rule){&cylinder->color, 0, 255, F_VEC3};
	cylinder_rules[5] = (t_field_rule){NULL, 0, 0, NONE};
	return (cylinder_rules);
}

static inline t_field_rule *ambient_rules(struct s_ambient *ambient)
{
	static t_field_rule ambient_rules[3];

	ambient_rules[0] = (t_field_rule){&ambient->ratio, 0.0, 1.0, F_VALUE};
	ambient_rules[1] = (t_field_rule){&ambient->color, 0, 255, F_VEC3};
	ambient_rules[2] = (t_field_rule){NULL, 0, 0, NONE};
	return (ambient_rules);
}

static inline t_field_rule *light_rules(struct s_light *light)
{
	static t_field_rule light_rules[4];

	light_rules[0] = (t_field_rule){&light->pos, -INF, INF, F_VEC3};
	light_rules[1] = (t_field_rule){&light->ratio, 0.0, 1.0, F_VALUE};
	light_rules[2] = (t_field_rule){&light->color, 0, 255, F_VEC3};
	light_rules[3] = (t_field_rule){NULL, 0, 0, NONE};
	return (light_rules);
}