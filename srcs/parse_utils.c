#include "minirt.h"

static int is_space(int c)
{
     return (c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\v'||c=='\f');
}

int parse_double(const char *s, double *out)
{
	char *end; double v;
	if (!s || !out) return (0);
	v = strtod(s, &end);
	if (end == s) return (0);
	while (*end && is_space(*end)) end++;
	if (*end) return (0);
	*out = v; return (1);
}

static int parse_int(const char *s, int *out)
{
	long v;
    char *end;
	if (!s || !out)
    return (0);
	v = strtol(s, &end, 10);
	if (end == s) 
    return (0);
	while (*end && is_space(*end)) 
    end++;
	if (*end) 
    return (0);
	*out = (int)v; 
    return (1);
}

int parse_in_range_d(double v, double minv, double maxv)
{ 
    return (v>=minv && v<=maxv); 
}
int parse_in_range_i(int v, int mini, int maxi)
{
     return (v>=mini && v<=maxi); 
}

int parse_vec3(const char *s, t_vec3 *v)
{
	char **p; 
    int ok;
	if (!s || !v) 
    return (0);
	p = ft_split(s, ','); 
    if (!p) 
    return (0);
	ok = (p[0] && p[1] && p[2] &&
		parse_double(p[0], &v->x) &&
		parse_double(p[1], &v->y) &&
		parse_double(p[2], &v->z));
	/* free split */
	int i = 0; 
    while (p[i]) 
    { 
        free(p[i]); 
        i++; 
    } 
    free(p);
	return (ok);
}

int parse_color(const char *s, t_color *c)
{
	char **p; 
    int r;
    int g;
    int b; 
    int ok;
	if (!s || !c) 
    return (0);
	p = ft_split(s, ','); 
    if (!p) 
    return (0);
	ok = (p[0] && p[1] && p[2] &&
		parse_int(p[0], &r) && parse_int(p[1], &g) && parse_int(p[2], &b) &&
		parse_in_range_i(r,0,255) && parse_in_range_i(g,0,255) && parse_in_range_i(b,0,255));
	if (ok)
    {
         c->r=r;
         c->g=g; 
         c->b=b; 
    }
	int i = 0; 
    while (p[i]) 
    { 
        free(p[i]); 
        i++; 
    }
    free(p);
	return (ok);
}

t_vec3 vnorm(t_vec3 v, int *ok)
{
	double len = v.x*v.x + v.y*v.y + v.z*v.z;
	t_vec3 z = {0,0,0};
	if (len <= 0.0)
    {
        if (ok)
        *ok = 0; 
        return z; 
    }
	len = sqrt(len);
	v.x /= len; 
    v.y /= len; 
    v.z /= len;
	if (ok) 
    *ok = 1; 
    return v;
}
