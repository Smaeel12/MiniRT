/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaafar <jaafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 21:37:50 by jaafar            #+#    #+#             */
/*   Updated: 2025/10/17 22:01:19 by jaafar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int is_ws(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
            || c == '\v' || c == '\f');
}
int parse_double(const char *s, double *out)
{
    char    *end;
    double  v;

    if (!s || !out)
        return (0);
    v = strtod(s, &end);
    if (end == s)
        return (0);
    while (*end && is_ws(*end))
        end++;
    if (*end)
        return (0);
    *out = v;
    return (1);
}
int parse_int(const char *s, int *out)
{
    long    v;
    char    *end;

    if (!s || !out)
        return (0);
    v = strtol(s, &end, 10);
    if (end == s)
        return (0);
    while (*end && is_ws(*end))
        end++;
    if (*end)
        return (0);
    *out = (int)v;
    return (1);
}
int in_range_d(double v, double mn, double mx)
{
     return (v >= mn && v <= mx); 
}
int in_range_i(int v, int mn, int mx) 
{ 
    return (v >= mn && v <= mx); 
}

/* parse "R,G,B" into t_color with range checks */
int parse_color(const char *s, t_color *c)
{
	char	**p;
	int		r;
	int		g;
	int		b;
	int		ok;
	int		i;

	if (!s || !c)
		return (0);
	p = ft_split(s, ',');
	if (!p)
		return (0);
	ok = (p[0] && p[1] && p[2]
		&& parse_int(p[0], &r)
		&& parse_int(p[1], &g)
		&& parse_int(p[2], &b)
		&& in_range_i(r, 0, 255)
		&& in_range_i(g, 0, 255)
		&& in_range_i(b, 0, 255));
	if (ok)
	{
		c->r = r;
		c->g = g;
		c->b = b;
	}
	i = 0;
	while (p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
	return (ok);
}


