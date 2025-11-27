#include "main.h"
#include "scene_parser.h"

// int init_scene(struct s_app *rt, int file)
// {
// 	void **compare_group;

// 	compare_group = (void *[]){
// 		{"C", (t_parse_rule * (*)(void *)) camera_rules, NULL},

// 		{"sp", (t_parse_rule * (*)(void *)) sphere_rules,
// 		 (t_hit (*)(union s_surface *, t_vec3, t_vec3))sphere_intersection},

// 		{"pl", (t_parse_rule * (*)(void *)) plane_rules,
// 		 (t_hit (*)(union s_surface *, t_vec3, t_vec3))plane_intersection},

// 		{"cy", (t_parse_rule * (*)(void *)) cylinder_rules,
// 		 (t_hit (*)(union s_surface *, t_vec3, t_vec3))cylinder_intersection},

// 		{NULL, NULL, NULL},
// 	};

// 	int i = 0;

// 	while ((char *)((void **)compare_group[i])[0]) {
// 		printf("%s\n", (char *)((void **)compare_group[i])[0]);
// 	}

// 	return 0;
// }

int init_scene(struct s_app *rt, int file)
{
	char *line;
	char *id;
	int i;

	i = 0;
	while (True) {
		line = get_next_line(file);
		if (!line)
			break;
		if (line && !*line)
			continue;
		id = ft_strsep(&line, ' ');
		if (id && line && !strcmp(id, "C") &&
			!parse_rules(line, camera_rules(&rt->scene.camera)))
			continue;
		else if (id && line && !strcmp(id, "sp") &&
				 !parse_rules(
					 line, sphere_rules((t_sphere *)&rt->scene.surfaces[i]))) {
			rt->scene.surfaces[i++].hit_function = (t_hit (*)(
				union s_surface *, t_vec3, t_vec3))sphere_intersection;
			continue;
		}
		else if (id && line && !strcmp(id, "pl") &&
				 !parse_rules(line,
							  plane_rules((t_plane *)&rt->scene.surfaces[i]))) {
			rt->scene.surfaces[i++].hit_function = (t_hit (*)(
				union s_surface *, t_vec3, t_vec3))plane_intersection;
			continue;
		}
		else if (id && line && !strcmp(id, "cy") &&
				 !parse_rules(
					 line,
					 cylinder_rules((t_cylinder *)&rt->scene.surfaces[i]))) {
			rt->scene.surfaces[i++].hit_function = (t_hit (*)(
				union s_surface *, t_vec3, t_vec3))cylinder_intersection;
			continue;
		}
		else
			return (printf("%s Invalid identifier\n", line), free(id), -1);
		free(id);
	}
	return (0);
}
