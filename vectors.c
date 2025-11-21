/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 22:30:55 by iboubkri          #+#    #+#             */
/*   Updated: 2025/11/10 02:12:10 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_color3 color3(double r, double g, double b)
{
	return ((t_color3){{r, g, b}});
}

t_vec3 vec3(double x, double y, double z)
{
	return ((t_vec3){{x, y, z}});
}

t_vec3 vmul(double num, t_vec3 vec)
{
	return ((t_vec3){{num * vec.x, num * vec.y, num * vec.z}});
}

double vdot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3 vsub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{a.x - b.x, a.y - b.y, a.z - b.z}});
}

t_vec3 vadd(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{a.x + b.x, a.y + b.y, a.z + b.z}});
}

t_vec3 vnorm(t_vec3 vec)
{
	double len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return ((t_vec3){{vec.x / len, vec.y / len, vec.z / len}});
}

t_vec3 vcross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	}});
}