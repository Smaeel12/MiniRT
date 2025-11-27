/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:17:38 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/06 15:11:17 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	char	*new;
	int		i;
	int		j;
	int		dlen;
	int		slen;

	i = 0;
	j = 0;
	dlen = ft_strlen(dest);
	slen = ft_strlen(src);
	new = (char *)malloc(dlen + slen + 2);
	while (dest[i])
	{
		new[i] = dest[i];
		i++;
	}
	new[i++] = ' ';
	while (src[j])
	{
		new[i + j] = src[j];
		j++;
	}
	new[i + j] = '\0';
	free(dest);
	return (new);
}
