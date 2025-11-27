#include "scene_parser.h"

#include "main.h"

int parse_vector(t_parse_rule rule, char *vector)
{
	float *input;
	char *value;
	int i;

	i	  = 0;
	input = rule.field;
	while (i < 3) {
		value = ft_strsep(&vector, ',');
		if (!*value)
			return (printf(INVALID_VECTOR), -1);
		input[i] = atof(value);
		if (input[i] > rule.max || input[i] < rule.min)
			return (printf(VALUE_OUT_RANGE), -1);
		i++;
	}
	return (0);
}

int parse_int(t_parse_rule rule, char *int_value)
{
	int *input;
	int i;

	i	  = 0;
	input = rule.field;
	while (int_value[i] && ((int_value[i] <= '9' && int_value[i] >= '0') ||
							int_value[i] == '.'))
		i++;
	if (int_value[i])
		return (printf(EXPECTED_INT_VALUE), -1);
	*input = atoi(int_value);
	if (*input > rule.max || *input < rule.min)
		return (printf(VALUE_OUT_RANGE), -1);
	return (0);
}

int parse_float(t_parse_rule rule, char *float_value)
{
	float *input;
	int i;

	i	  = 0;
	input = rule.field;
	while (float_value[i] &&
		   ((float_value[i] <= '9' && float_value[i] >= '0') ||
			float_value[i] == '.'))
		i++;
	if (float_value[i])
		return (printf(EXPECTED_FLOAT_VALUE), -1);
	*input = atof(float_value);
	if (*input > rule.max || *input < rule.min)
		return (printf(VALUE_OUT_RANGE), -1);
	return (0);
}

int parse_rules(char *tokens, t_parse_rule rules[])
{
	char *value;
	int i;

	i = 0;
	while (rules[i].field) {
		value = ft_strsep(&tokens, ' ');
		if (!*value)
			return (printf(MISSING_FIELD), -1);
		if (rules[i].type == F_VEC3 && parse_vector(rules[i], value))
			return (-1);
		if (rules[i].type == F_INT && parse_int(rules[i], value))
			return (-1);
		if (rules[i].type == F_FLOAT && parse_float(rules[i], value))
			return (-1);
		i++;
	}
	return (0);
}
