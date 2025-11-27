#include "main.h"

#define MISSING_FIELD "missing field\n"

#define INVALID_VECTOR "Invalid vector\n"
#define VALUE_OUT_RANGE "Value out of range\n"
#define EXPECTED_INT_VALUE "expected int value\n"
#define EXPECTED_FLOAT_VALUE "expected float value\n"

enum e_field_type { NONE, F_INT, F_FLOAT, F_VEC3 };
typedef struct {
	void *field;
	enum e_field_type type;
	float min;
	float max;
} t_parse_rule;

static inline t_parse_rule *camera_rules(struct s_camera *camera)
{
	static t_parse_rule camera_rules[4];

	camera_rules[0] = (t_parse_rule){&camera->pos, F_VEC3, -INF, INF};
	camera_rules[1] = (t_parse_rule){&camera->fd, F_VEC3, 0.0, 1.0};
	camera_rules[2] = (t_parse_rule){&camera->fov, F_INT, 0, 180};
	camera_rules[3] = (t_parse_rule){NULL, NONE, 0, 0};
	return (camera_rules);
}

static inline t_parse_rule *sphere_rules(t_sphere *sphere)
{
	static t_parse_rule sphere_rules[3];

	sphere_rules[0] = (t_parse_rule){&sphere->pos, F_VEC3, -INF, INF};
	sphere_rules[1] = (t_parse_rule){&sphere->diameter, F_FLOAT, -INF, INF};
	sphere_rules[2] = (t_parse_rule){&sphere->color, F_VEC3, 0, 255};
	return (sphere_rules);
}

static inline t_parse_rule *plane_rules(t_plane *plane)
{
	static t_parse_rule plane_rules[3];

	plane_rules[0] = (t_parse_rule){&plane->pos, F_VEC3, -INF, INF};
	plane_rules[1] = (t_parse_rule){&plane->normal, F_VEC3, 0.0, 1.0};
	plane_rules[2] = (t_parse_rule){&plane->color, F_VEC3, 0, 255};
	return (plane_rules);
}

static inline t_parse_rule *cylinder_rules(t_cylinder *cylinder)
{
	static t_parse_rule cylinder_rules[5];

	cylinder_rules[0] = (t_parse_rule){&cylinder->pos, F_VEC3, -INF, INF};
	cylinder_rules[1] = (t_parse_rule){&cylinder->normal, F_VEC3, 0.0, 1.0};
	cylinder_rules[2] = (t_parse_rule){&cylinder->diameter, F_FLOAT, -INF, INF};
	cylinder_rules[3] = (t_parse_rule){&cylinder->height, F_FLOAT, -INF, INF};
	cylinder_rules[4] = (t_parse_rule){&cylinder->color, F_VEC3, 0, 255};
	return (cylinder_rules);
}

int parse_rules(char *tokens, t_parse_rule rules[]);
int parse_float(t_parse_rule rule, char *float_value);
int parse_int(t_parse_rule rule, char *int_value);
int parse_vector(t_parse_rule rule, char *vector);
