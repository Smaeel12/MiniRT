/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 05:16:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/20 23:15:35 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../mlx/mlx.h"
#include "../mlx/mlx_int.h"
#include "vectors.h"

union u_surface;

typedef struct {
	union u_surface *surface;
	t_vec3 normal;
	t_vec3 p;
	float t;
} t_hit;

typedef struct {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_color3 color;

	t_vec3 pos;
	t_vec3 axis;
	float diameter;
	float height;
} t_cylinder;

typedef struct {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_color3 color;

	t_vec3 pos;
	t_vec3 normal;
} t_plane;

typedef struct {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_color3 color;

	t_vec3 pos;
	float diameter;
} t_sphere;

typedef union u_surface {
	struct {
		t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
		t_color3 color;
	};
	t_cylinder cylinder;
	t_sphere sphere;
	t_plane plane;
} t_surface;

#ifndef MAX_SURFACES
#define MAX_SURFACES 15
#endif

struct s_raytracer {
	struct s_scene {
		t_surface surfaces[MAX_SURFACES];
		struct s_camera {
			t_vec3 pos;
			t_vec3 u, v, w;
			float fov;
			struct {
				t_vec3 delta_u, delta_v, px00loc;
			} viewport;
		} camera;
		struct s_ambient {
			t_vec3 color;
			float ratio;
		} ambient;
		struct s_light {
			t_vec3 color;
			t_vec3 pos;
			float ratio;
		} light;
	} scene;
	struct s_window {
		t_img *frame;
		t_win_list *ptr;
		int height;
		int width;
	} win;
	t_xvar *mlx;
	t_vec3 *acumm;
	int nsamples;
};

enum e_field_type {
	NONE,
	F_VEC3,
	F_VALUE,
};

typedef struct {
	void *field;
	float min;
	float max;
	enum e_field_type type;
} t_field_rule;

typedef struct {
	char *field_name;
	int nchar;
	void *object;
	t_field_rule *(*rules)(void *);
} t_parser_rule;
