/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 17:27:34 by iboubkri          #+#    #+#             */
/*   Updated: 2025/11/27 12:32:36 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*join_buffers(char *lbuf, char *rbuf)
{
	char	*new;

	if (!*rbuf)
		return (lbuf);
	if (!lbuf)
		new = ft_strdup(rbuf);
	else
		new = ft_strjoin(lbuf, rbuf);
	return (free(lbuf), new);
}

char	*return_line(char **lbuf, char *pos)
{
	char	*temp;

	temp = ft_substr(*lbuf, 0, pos - *lbuf);
	if (!temp)
		return (free(*lbuf), *lbuf = NULL, NULL);
	if (ft_strlcpy(*lbuf, pos + 1, ft_strlen(pos + 1) + 1) == 0)
	{
		free(*lbuf);
		*lbuf = NULL;
	}
	return (temp);
}

char	*get_next_line(int fd)
{
	static char	*lbuf;
	char		*rbuf;
	ssize_t		nb;

	nb = 0;
	rbuf = (char *)malloc((size_t)BUFFER_SIZE + 1);
	while (rbuf)
	{
		rbuf[nb] = '\0';
		lbuf = join_buffers(lbuf, rbuf);
		if (lbuf && ft_strchr(lbuf, '\n'))
		{
			free(rbuf);
			return (return_line(&lbuf, ft_strchr(lbuf, '\n')));
		}
		nb = read(fd, rbuf, BUFFER_SIZE);
		if (nb == -1)
			return (free(rbuf), free(lbuf), lbuf = NULL, NULL);
		if (!nb)
			return (free(rbuf), rbuf = lbuf, lbuf = NULL, rbuf);
	}
	return (free(lbuf), lbuf = NULL, NULL);
}
