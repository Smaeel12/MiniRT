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
	return (sqrt(VLENGHT_SQUARED(vec)));
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
	return ((t_vec3){n * vec.x, n * vec.y, n * vec.z});
}

#define VADD vadd
static inline t_vec3 vadd(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

#define VSUB vsub
static inline t_vec3 vsub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

#define VDOT vdot
static inline float vdot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

#define VCROSS vcross
static inline t_vec3 vcross(t_vec3 a, t_vec3 b)
{
	return (t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	};
}

#define CLAMP clamp
static inline float clamp(float value)
{
	if (value > 0.999)
		return 1.0;
	else if (value < 0.0)
		return 0.0;
	return value;
}

#define HADAMARD_PRODUCT hadamard_product
static inline t_vec3 hadamard_product(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x * b.x, a.y * b.y, a.z * b.z});
}

#define RANF ranf
static inline float ranf(float min, float max)
{
	return min + (max - min) * rand() / (RAND_MAX + 1.0);
}

#define EPS 0.001
