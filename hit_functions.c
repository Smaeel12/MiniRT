#include "includes/structs.h"
#include "includes/vectors.h"

t_hit plane_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_plane *plane = (t_plane *)surface++;

	t_hit hit = ((t_hit){NULL, (t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});

	if (surface->hit_function)
		hit = surface->hit_function(surface, org, dir);

	float denom = VDOT(plane->normal, dir);
	if (fabs(denom) < 1e-6)
		return hit;

	t_vec3 oc  = VSUB(plane->pos, org);
	float root = VDOT(oc, plane->normal) / denom;

	if (root < 1e-6 || root > hit.t)
		return hit;

	float normal_direction = (denom < 0) ? 1.0f : -1.0f;

	return ((t_hit){(t_surface *)plane, VMUL(normal_direction, plane->normal),
					VADD(org, VMUL(root, dir)), root});
}

t_hit sphere_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_sphere *sphere = (t_sphere *)surface++;

	t_hit hit = ((t_hit){NULL, (t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});

	if (surface->hit_function)
		hit = surface->hit_function(surface, org, dir);

	t_vec3 oc = VSUB(sphere->pos, org);
	float a	  = VDOT(dir, dir);
	float b	  = -2.0f * VDOT(dir, oc);

	float radius = sphere->diameter / 2;
	float c		 = VDOT(oc, oc) - radius * radius;

	float disc = b * b - 4 * a * c;

	if (disc < 0.0)
		return hit;

	float sqrt_disc = sqrt(disc);

	float root = (-b - sqrt_disc) / (2.0f * a);
	if (root < 0.001)
		root = (-b + sqrt_disc) / (2.0f * a);
	if (root < 0.001 || root > hit.t)
		return hit;

	t_vec3 hit_point = VADD(org, VMUL(root, dir));
	t_vec3 normal	 = VMUL(1 / radius, VSUB(hit_point, sphere->pos));

	return (((t_hit){(t_surface *)sphere, normal, hit_point, root}));
}

t_hit cylinder_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_cylinder *cylinder = (t_cylinder *)surface++;

	t_hit hit = ((t_hit){NULL, (t_vec3){-1, -1, -1}, (t_vec3){0, 0, 0}, INF});

	if (surface->hit_function)
		hit = surface->hit_function(surface, org, dir);

	// cylinder->axis = VNORM(cylinder->axis);
	t_vec3 oc	   = VSUB(org, cylinder->pos);
	float dot_dv   = VDOT(dir, cylinder->axis);
	float dot_xv   = VDOT(oc, cylinder->axis);

	float a		 = VDOT(dir, dir) - dot_dv * dot_dv;
	float b		 = 2.0f * (VDOT(dir, oc) - dot_dv * dot_xv);
	float radius = cylinder->diameter / 2.0f;

	float c = VDOT(oc, oc) - (dot_xv * dot_xv) - radius * radius;

	float disc = b * b - 4 * a * c;
	if (disc < 0.0)
		return hit;

	float sqrt_disc = sqrt(disc);

	float root = (-b - sqrt_disc) / (2.0f * a);
	float m	   = dot_dv * root + dot_xv;
	if (root < 1e-6 || m < 0 || m > cylinder->height) {
		root = (-b + sqrt_disc) / (2.0f * a);
		m	 = dot_dv * root + dot_xv;
		if (root < 1e-6 || m < 0 || m > cylinder->height)
			return hit;
	}

	if (root > hit.t)
		return hit;

	t_vec3 hit_point = VADD(org, VMUL(root, dir));

	t_vec3 normal =
		VNORM(VSUB(hit_point, VADD(cylinder->pos, VMUL(m, cylinder->axis))));

	if (VDOT(dir, normal) > 0)
		normal = VMUL(-1, normal);

	return ((t_hit){(t_surface *)cylinder, normal, hit_point, root});
}