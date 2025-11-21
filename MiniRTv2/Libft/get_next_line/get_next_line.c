/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 17:27:34 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/07 00:42:18 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char *ft_strjoin_helper(char *lbuf, char *rbuf)
{
	char *new;

	if (!lbuf)
		new = ft_strdup(rbuf);
	else
		new = ft_strjoin(lbuf, rbuf);
	return (free(lbuf), new);
}

char *return_line(char **lbuf, char *pos)
{
	char *temp;

	temp = ft_substr(*lbuf, 0, pos - *lbuf + 1);
	if (!temp)
		return (free(*lbuf), *lbuf = NULL, NULL);
	if (ft_strlcpy(*lbuf, pos + 1, ft_strlen(pos)) == 0)
	{
		free(*lbuf);
		*lbuf = NULL;
	}
	return (temp);
}

char *get_next_line(int fd)
{
	static char *lbuf;
	char *rbuf;
	ssize_t nb;

	rbuf = (char *)malloc((size_t)BUFFER_SIZE + 1);
	while (rbuf)
	{
		nb = read(fd, rbuf, BUFFER_SIZE);
		if (nb == -1)
			return (free(rbuf), free(lbuf), lbuf = NULL, NULL);
		rbuf[nb] = '\0';
		if (!nb)
			return (free(rbuf), rbuf = lbuf, lbuf = NULL, rbuf);
		lbuf = ft_strjoin_helper(lbuf, rbuf);
		if (lbuf && ft_strchr(lbuf, '\n'))
		{
			free(rbuf);
			return (return_line(&lbuf, ft_strchr(lbuf, '\n')));
		}
	}
	return (free(lbuf), lbuf = NULL, NULL);
}
