/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 22:29:43 by iboubkri          #+#    #+#             */
/*   Updated: 2025/10/24 16:24:25 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <math.h>

typedef union {
	struct {
		double x, y, z;
	};
	struct {
		double r, g, b;
	};
} t_vec3;

typedef t_vec3 t_color3;

typedef struct {
	double t;
	t_vec3 n;
} t_hit;

typedef struct s_surface {
	t_hit (*hit_func)(struct s_surface *, t_vec3, t_vec3);
	struct s_surface *next;
} t_surface;

typedef struct {
	t_hit (*hit_func)(t_surface *, t_vec3, t_vec3);
	t_surface *next;

	t_color3 color;
	t_vec3 pos;
	double radius;
} t_sphere;

typedef struct {
	t_hit (*hit_func)(t_surface *, t_vec3, t_vec3);
	t_surface *next;

	t_color3 color;
	t_vec3 point[3];
} t_triangle;

typedef struct {
	t_surface *surface;

	t_vec3 camera;
} t_scene;

typedef struct {
	void *buffer;
	void *ptr;
	void *win;

	int w;
	int h;

	t_scene scene;
} t_mlx;

#define COLOR3 color3
#define VEC3 vec3
#define VDOT vdot
#define VMUL vmul
#define VSUB vsub
#define VADD vadd
#define VNORM vnorm

t_color3 color3(double r, double g, double b);
t_vec3 vec3(double a, double b, double c);
t_vec3 vmul(double num, t_vec3 vec);
double vdot(t_vec3 a, t_vec3 b);
t_vec3 vsub(t_vec3 a, t_vec3 b);
t_vec3 vadd(t_vec3 a, t_vec3 b);
t_vec3 vnorm(t_vec3 vec);

#endif