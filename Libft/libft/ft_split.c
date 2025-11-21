/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:37:55 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/06 15:11:17 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t count_words(char const *s, char c)
{
	size_t count;
	size_t slen;
	size_t i;

	count = 0;
	i = 0;
	slen = ft_strlen(s);
	while (i < slen + 1)
	{
		if ((s[i] == c || !s[i]) && (i > 0 && s[i - 1] && s[i - 1] != c))
			count++;
		i++;
	}
	return (count);
}

static void free_all(char **arr)
{
	size_t i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

static void fill_array(char **arr, char const *s, char c)
{
	size_t i;
	size_t j;
	size_t k;

	i = 0;
	k = 0;
	while (s[i])
	{
		j = 0;
		while (s[i + j] && s[i + j] != c)
			j++;
		if (j)
		{
			arr[k] = ft_substr(s, i, j);
			if (!arr[k])
			{
				free_all(arr);
				return;
			}
			i = i + j - 1;
			k++;
		}
		i++;
	}
	arr[k] = NULL;
}

char **ft_split(char const *s, char c)
{
	char **arr;
	size_t num;

	if (!s)
		return (NULL);
	num = count_words(s, c) + 1;
	arr = malloc(num * sizeof(char *));
	if (!arr)
		return (NULL);
	fill_array(arr, s, c);
	return (arr);
}
