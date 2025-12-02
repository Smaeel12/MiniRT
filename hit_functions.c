#include "main.h"

t_hit	plane_intersection(t_plane *plane, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){0, (t_vec3){0, 0, 0}});
}

t_hit	sphere_intersection(t_sphere *sphere, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){0, (t_vec3){0, 0, 0}});
}

t_hit	cylinder_intersection(t_cylinder *cylinder, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){0, (t_vec3){0, 0, 0}});
}
