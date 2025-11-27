/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:13:26 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/06 15:11:18 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	slen;
	char	*new;

	if (!s || !f)
		return (NULL);
	slen = ft_strlen(s);
	new = (char *)malloc((slen + 1) * sizeof(char));
	if (!new)
		return (NULL);
	new[slen] = '\0';
	while (slen--)
	{
		new[slen] = f(slen, s[slen]);
	}
	return (new);
}
