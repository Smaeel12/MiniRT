#pragma once

#include <math.h>

#define FLT_EPSILION 1e-5
#define INF INFINITY

typedef struct {
	float x, y, z;
} t_vec3;

typedef t_vec3 t_color3;

#define VLENGHT_SQUARED vlenght_squared
static inline float vlenght_squared(t_vec3 vec)
{
	return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

#define VLENGHT vlenght
static inline float vlenght(t_vec3 vec)
{
	return sqrt(VLENGHT_SQUARED(vec));
}

#define VNORM vnorm
static inline t_vec3 vnorm(t_vec3 vec)
{
	double len;

	len = VLENGHT(vec);
	return ((t_vec3){vec.x / len, vec.y / len, vec.z / len});
}

#define VMUL vmul
static inline t_vec3 vmul(float n, t_vec3 vec)
{
	return (t_vec3){n * vec.x, n * vec.y, n * vec.z};
}

#define VADD vadd
static inline t_vec3 vadd(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

#define VSUB vsub
static inline t_vec3 vsub(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

#define VDOT vdot
static inline float vdot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
