/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 04:45:04 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/30 23:52:42 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parser.h"

int	parse_value(t_field_rule rule, char *float_value)
{
	float	*input;
	float	v;

	input = rule.field;
	if (!float_value || !*float_value)
		return (fallback_message(ERR_EXPECTED_FLOAT), -1);
	v = ft_atof(float_value);
	if (!isfinite(v))
		return (fallback_message(ERR_VALUE_OUT_RANGE), -1);
	if (v > rule.max || v < rule.min)
		return (fallback_message(ERR_VALUE_OUT_RANGE), -1);
	*input = v;
	return (0);
}

int	parse_vector(t_field_rule rule, char *vector)
{
	float	*input;
	char	*value;
	int		i;
	float	v;

	input = rule.field;
	i = 0;
	while (i < 3)
	{
		value = ft_strsep(&vector, ',');
		if (!value || !*value)
			return (fallback_message(ERR_EXPECTED_VECTOR), -1);
		v = ft_atof(value);
		if (!isfinite(v))
			return (fallback_message(ERR_VALUE_OUT_RANGE), -1);
		if (v > rule.max || v < rule.min)
			return (fallback_message(ERR_VALUE_OUT_RANGE), -1);
		input[i] = v;
		i++;
	}
	if (vector && *vector)
		return (fallback_message(ERR_EXPECTED_VECTOR), -1);
	return (0);
}

int	parse_fields(char *tokens, t_field_rule rules[])
{
	char	*value;
	int		i;

	i = 0;
	while (true)
	{
		value = ft_strsep(&tokens, ' ');
		if (!rules[i].field && !value)
			break ;
		if (!value || !*value)
			return (fallback_message(ERR_MISSING_FIELD), -1);
		if (!rules[i].field)
			return (fallback_message(ERR_UNEXPECTED_FIELD), -1);
		if (rules[i].type == F_VEC3 && parse_vector(rules[i], value))
			return (-1);
		else if (rules[i].type == F_VALUE && parse_value(rules[i], value))
			return (-1);
		i++;
	}
	return (0);
}

t_parser_rule	*parser_rules(struct s_scene *scene)
{
	static t_parser_rule	parser_rules[7];

	parser_rules[0] = (t_parser_rule){(t_field_rule
			* (*)(void *)) ambient_rules, &scene->ambient, "A", 0, 0};
	parser_rules[1] = (t_parser_rule){(t_field_rule * (*)(void *)) camera_rules,
		&scene->camera, "C", 0, 0};
	parser_rules[2] = (t_parser_rule){(t_field_rule * (*)(void *)) light_rules,
		&scene->light, "L", 0, 0};
	parser_rules[3] = (t_parser_rule){(t_field_rule * (*)(void *)) sphere_rules,
		scene->surfaces, "sp", 0, sizeof(t_surface)};
	parser_rules[4] = (t_parser_rule){(t_field_rule * (*)(void *)) plane_rules,
		scene->surfaces, "pl", 0, sizeof(t_surface)};
	parser_rules[5] = (t_parser_rule){(t_field_rule
			* (*)(void *)) cylinder_rules, scene->surfaces, "cy", 0,
		sizeof(t_surface)};
	parser_rules[6] = (t_parser_rule){0};
	return (parser_rules);
}

int	parse_file(int fd, t_parser_rule rules[])
{
	char *(line), *(head), *(id);
	int (curr), (i);
	curr = 0;
	line = get_next_line(fd);
	while (line)
	{
		i = 0;
		head = line;
		id = ft_strsep(&line, ' ');
		while (id && rules[i].name && ft_strcmp(rules[i].name, id))
			i++;
		if (id && ++rules[i].count && !rules[i].name)
			return (fallback_message(ERR_UNKNOWN_OBJECT), 1);
		if (id && id[0] >= 'A' && id[0] <= 'Z' && rules[i].count > 1)
			return (fallback_message(ERR_REDEFINE_OBJECT), 1);
		if (id && parse_fields(line, rules[i].func((char *)rules[i].object
					+ curr * rules[i].size)))
			return (1);
		curr += id && !(id[0] >= 'A' && id[0] <= 'Z') && (curr
				+ 1 < MAX_SURFACES - 1);
		line = get_next_line(fd);
		free(head);
	}
	return (0);
}
