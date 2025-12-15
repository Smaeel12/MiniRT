#include "includes/structs.h"
#include "includes/vectors.h"

t_hit plane_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){(t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});
}

t_hit sphere_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_sphere *sphere = (t_sphere *)surface++;

	t_hit hit = ((t_hit){(t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});
	if (surface->hit_function)
		hit = surface->hit_function(surface, org, dir);

	t_vec3 oc = VSUB(sphere->pos, org);
	double a  = VDOT(dir, dir);
	double b  = -2.0 * VDOT(dir, oc);
	double disc =
		b * b - 4 * a * (VDOT(oc, oc) - sphere->radius * sphere->radius);

	if (disc < 0.0)
		return hit;

	double root = (-b - sqrt(disc)) / (2.0 * a);
	if (root < 0.001)
		root = (-b + sqrt(disc)) / (2.0 * a);
	if (root < 0.001)
		return hit;
	return (((t_hit){VNORM(VSUB(VADD(org, VMUL(root, dir)), sphere->pos)),
					 VADD(org, VMUL(root, dir)), root}));
}

t_hit cylinder_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){(t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});
}
