#include "includes/parser.h"

t_parser_rule *parser_rules(struct s_scene *scene)
{
	static t_parser_rule parser_rules[5];

	parser_rules[0] = (t_parser_rule){
		"C", 2, &scene->camera, (t_field_rule * (*)(void *)) camera_rules};
	parser_rules[1] = (t_parser_rule){"pl", 3, scene->surfaces,
									  (t_field_rule * (*)(void *)) plane_rules};
	parser_rules[2] = (t_parser_rule){
		"sp", 3, scene->surfaces, (t_field_rule * (*)(void *)) sphere_rules};
	parser_rules[3] = (t_parser_rule){
		"cy", 3, scene->surfaces, (t_field_rule * (*)(void *)) cylinder_rules};
	parser_rules[4] = (t_parser_rule){NULL, 0, NULL, NULL};
	return (parser_rules);
}

int parse_value(t_field_rule rule, char *float_value)
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
		return fallback_message(ERR_EXPECTED_FLOAT), -1;

	*input = atof(float_value);	 // atof needs implement
	if (*input > rule.max || *input < rule.min)
		return fallback_message(ERR_VALUE_OUT_RANGE), -1;

	return (0);
}
int parse_vector(t_field_rule rule, char *vector)
{
	float *input;
	char *value;
	int i;

	i	  = 0;
	input = rule.field;
	while (i < 3) {
		value = ft_strsep(&vector, ',');
		if (!value || !*value)
			return fallback_message(ERR_EXPECTED_VECTOR), -1;

		input[i] = atof(value);	 // atof needs implement
		if (input[i] > rule.max || input[i] < rule.min)
			return fallback_message(ERR_VALUE_OUT_RANGE), -1;

		i++;
	}
	return (0);
}
int parse_fields(char *tokens, t_field_rule rules[])
{
	char *value;
	int i;

	i = 0;
	while (true) {
		value = ft_strsep(&tokens, ' ');
		if (!rules[i].field && !value)
			break;
		if (!value || !*value)
			return fallback_message(ERR_MISSING_FIELD), -1;
		if (!rules[i].field)
			return fallback_message(ERR_UNEXPECTED_FIELD), -1;
		if (rules[i].type == F_VEC3 && parse_vector(rules[i], value))
			return (-1);
		else if (rules[i].type == F_VALUE && parse_value(rules[i], value))
			return (-1);
		i++;
	}
	return (0);
}

int parse_objects(int fd, t_parser_rule rules[])
{
	char *line;
	char *id;
	int curr;
	int i;

	curr = 0;
	if (fd < 0)
		return (fallback_message(ERR_FAILED_OPEN), exit(-1), -1);

	while (true) {
		i	 = 0;
		line = get_next_line(fd);
		if (!line)
			break;
		id = ft_strsep(&line, ' ');
		if (!id) {
			free(line);
			continue;
		}
		while (rules[i].field_name &&
			   ft_strncmp(rules[i].field_name, id, rules[i].nchar))
			i++;
		if (!rules[i].field_name)
			return (close(fd), free(id), free(rules[1].object),
					fallback_message(ERR_UNKNOWN_OBJECT), exit(-1), -1);

		if (parse_fields(line, ((void *(*)(void *))rules[i].rules)(
								   rules[i].object + (curr * (id[0] != 'C') *
													  sizeof(t_surface)))))
			return (close(fd), free(id), free(rules[1].object), exit(-1), -1);

		curr += (id[0] != 'C' && curr < MAX_SURFACES - 1);
		free(id);
	}
	close(fd);
	return (0);
}
