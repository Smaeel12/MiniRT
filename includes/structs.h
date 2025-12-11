/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 05:16:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/11 05:28:40 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../mlx/mlx.h"
#include "../mlx/mlx_int.h"
#include "vectors.h"

typedef struct {
	t_vec3 normal;
	float t;
} t_hit;

union u_surface;

typedef struct {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_vec3 pos;
	t_vec3 normal;
	float diameter;
	float height;
	t_color3 color;
} t_cylinder;

typedef struct {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_vec3 pos;
	t_vec3 normal;
	t_color3 color;
} t_plane;

typedef struct {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_vec3 pos;
	float radius;
	t_color3 color;
} t_sphere;

typedef struct {
	t_vec3 color;
	float ratio;
} t_ambiant;

typedef struct {
	t_vec3 color;
	t_vec3 pos;
	float ratio;
} t_light;

typedef union u_surface {
	t_hit (*hit_function)(union u_surface *, t_vec3, t_vec3);
	t_cylinder cylinder;
	t_sphere sphere;
	t_plane plane;
} t_surface;

struct s_raytracer {
	struct s_scene {
		struct s_camera {
			t_vec3 fd;
			t_vec3 up;
			t_vec3 rt;
			t_vec3 pos;
			float fov;
			struct {
				t_vec3 delta_u, delta_v, px00loc;
			} viewport;

		} camera;
		t_surface *surfaces;
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
