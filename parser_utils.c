#include "parser_utils.h"

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

int parse_objects(int file, t_parser_rule rules[])
{
    char *line;
    char *id;
    int curr;
    int i;

    curr = 0;
    while (true)
    {
        i = 0;
        line = get_next_line(file);
        if (!line)
            break;
        id = ft_strsep(&line, ' ');
        while (id && rules[i].field_name &&
               ft_strncmp(rules[i].field_name, id, rules[i].nchar))
            i++;
        if (id && !rules[i].field_name)
            on_close(ERR_UNKNOWN_OBJECT);
        if (id && parse_fields(line, ((void *(*)(void *))rules[i].rules)(
                                         rules[i].object +
                                         (curr * (id && id[0] != 'C')))))
            on_close(ERR_FAILED_PARSER);
        curr += id && id[0] != 'C' && curr < MAX_SURFACES;
        free(id);
    }
    return (0);
}
