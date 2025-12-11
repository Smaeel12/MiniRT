#pragma once

#include <stdbool.h>

#include "../Libft/libft.h"
#include "structs.h"

#ifndef MAX_SURFACES
#define MAX_SURFACES 15
#endif

#define ERR_NO_FILE \
	"Usage Error: No input file provided. Please specify a scene file.\n"

#define ERR_FAILED_OPEN                                                \
	"System Error: Could not open the specified file. Check path and " \
	"permissions.\n"

#define ERR_FAILED_PARSER \
	"Fatal Error: Parsing aborted. See previous logs for details.\n"

#define ERR_MISSING_FIELD "Syntax Error: A required field is missing.\n"

#define ERR_VALUE_OUT_RANGE \
	"Domain Error: The provided value is outside the allowable range.\n"

#define ERR_EXPECTED_FLOAT "Type Error: Expected a decimal number.\n"

#define ERR_EXPECTED_VECTOR \
	"Format Error: Expected a vector in format 'x,y,z' or 'r,g,b'.\n"

#define ERR_UNKNOWN_OBJECT "Syntax Error: Unknown object identifier.\n"

#define ERR_UNEXPECTED_FIELD \
	"Syntax Error: Unexpected extra field(s) provided for an object.\n"

void fallback_message(char *message);

t_parser_rule *parser_rules(struct s_scene *scene);
int parse_vector(t_field_rule rule, char *vector);
int parse_objects(int fd, t_parser_rule rules[]);
int parse_fields(char *tokens, t_field_rule rules[]);
int parse_value(t_field_rule rule, char *float_value);

t_hit sphere_intersection(union u_surface *surface, t_vec3 org, t_vec3 dir);
t_hit plane_intersection(union u_surface *surface, t_vec3 org, t_vec3 dir);
t_hit cylinder_intersection(union u_surface *surface, t_vec3 org, t_vec3 dir);

#include "parser_rules.h"