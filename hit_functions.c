/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 04:39:56 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/30 23:53:02 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/structs.h"
#include "includes/vectors.h"

int	solve_quadratic(float a, float b, float c, float roots[])
{
	float	disc;
	float	sqrt_disc;

	roots[0] = INF;
	roots[1] = INF;
	disc = b * b - 4 * a * c;
	if (disc < 0.0)
		return (1);
	sqrt_disc = sqrt(disc);
	roots[0] = (-b - sqrt_disc) / (2.0f * a);
	roots[1] = (-b + sqrt_disc) / (2.0f * a);
	if (roots[0] < EPS && roots[1] > EPS)
	{
		roots[0] = roots[1];
		roots[1] = INF;
	}
	return (0);
}

t_hit	plane_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_plane	*plane;
	t_hit	hit;
	t_vec3	oc;
	float	denom;
	float	root;

	plane = (t_plane *)surface++;
	hit = (t_hit){NULL, {0}, {0}, INF};
	if (surface && surface->hit_function)
		hit = surface->hit_function(surface, org, dir);
	denom = VDOT(dir, plane->normal);
	if (fabs(denom) < EPS)
		return (hit);
	oc = VSUB(plane->pos, org);
	root = VDOT(oc, plane->normal) / denom;
	if (root < EPS || root > hit.t)
		return (hit);
	hit = (t_hit){(t_surface *)plane, plane->normal, VADD(org, VMUL(root, dir)),
		root};
	if (denom > 0)
		hit.normal = VMUL(-1, plane->normal);
	return (hit);
}

t_hit	sphere_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_sphere	*sphere;
	t_hit		hit;
	t_vec3		oc;
	float		radius;
	float		roots[2];

	sphere = (t_sphere *)surface++;
	hit = (t_hit){NULL, {0}, {0}, INF};
	if (surface && surface->hit_function)
		hit = surface->hit_function(surface, org, dir);
	radius = sphere->diameter / 2;
	oc = VSUB(sphere->pos, org);
	if (solve_quadratic(VDOT(dir, dir), -2.0f * VDOT(dir, oc), VDOT(oc, oc)
			- radius * radius, roots) || roots[0] < EPS || roots[0] > hit.t)
		return (hit);
	hit.p = VADD(org, VMUL(roots[0], dir));
	return (((t_hit){(t_surface *)sphere, VMUL(1 / radius, VSUB(hit.p,
					sphere->pos)), hit.p, roots[0]}));
	return (hit);
}

t_hit	cylinder_intersection(t_surface *surface, t_vec3 org, t_vec3 dir)
{
	t_cylinder *(cy) = (t_cylinder *)surface++;
	t_vec3 (oc) = VSUB(org, cy->pos), dist;
	t_hit (hit) = {0, {0}, {0}, INF};
	float (roots[2]), (r) = cy->diameter / 2.f, (h) = cy->height / 2.f, (m),
		(tc), (s), (dv) = VDOT(dir, cy->axis), (xv) = VDOT(oc, cy->axis);
	int (i) = -1;
	if (surface && surface->hit_function)
		hit = surface->hit_function(surface, org, dir);
	solve_quadratic(1 - dv * dv, 2 * (VDOT(dir, oc) - dv * xv), VDOT(oc, oc)
		- xv * xv - r * r, roots);
	while (++i < 2)
	{
		m = xv + roots[i] * dv;
		if (roots[i] > EPS && roots[i] < hit.t && m >= -h && m <= h)
			hit = (t_hit){(t_surface *)cy, VNORM(VSUB(VADD(oc, VMUL(roots[i],
								dir)), VMUL(m, cy->axis))), VADD(org,
					VMUL(roots[i], dir)), roots[i]};
		s = 1.0f - 2.0f * i;
		tc = (s * h - xv) / dv;
		dist = VSUB(VADD(oc, VMUL(tc, dir)), VMUL(s * h, cy->axis));
		if (tc > EPS && tc < hit.t && VDOT(dist, dist) <= r * r)
			hit = (t_hit){(t_surface *)cy, VMUL(s, cy->axis), VADD(org, VMUL(tc,
						dir)), tc};
	}
	return (hit);
}
