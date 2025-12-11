#include "includes/structs.h"
#include "includes/vectors.h"

t_hit plane_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){(t_vec3){-1, -1, -1}, INFINITY});
}

t_hit sphere_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_sphere *sphere = (t_sphere *)surface++;

	t_hit hit = ((t_hit){(t_vec3){-1, -1, -1}, INFINITY});
	if (surface->hit_function)
		hit = surface->hit_function(surface, org, dir);

	t_vec3 oc = VSUB(sphere->pos, org);
	double a  = VDOT(dir, dir);
	double b  = -2.0 * VDOT(dir, oc);
	double disc =
		b * b - 4 * a * (VDOT(oc, oc) - sphere->radius * sphere->radius);

	if (disc < 0.0)
		return hit;

	double t = (-b - sqrt(disc)) / (2.0 * a);
	if (t < 0.0 + FLT_EPSILION)
		t = (-b + sqrt(disc)) / (2.0 * a);
	if (hit.t < t || t < 0.0 + FLT_EPSILION)
		return hit;
	return (((t_hit){VNORM(VSUB(VADD(org, VMUL(t, dir)), sphere->pos)), t}));
}

t_hit cylinder_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	return ((t_hit){(t_vec3){-1, -1, -1}, 0});
}
