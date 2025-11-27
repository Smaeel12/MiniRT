/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:55:08 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/06 15:11:18 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	putnbr_to_buffer(unsigned int n, int fd)
{
	char	c;

	if (!n)
		return ;
	c = (n % 10) + '0';
	putnbr_to_buffer(n / 10, fd);
	write(fd, &c, 1);
	return ;
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	num;

	if (!n)
	{
		write(fd, "0", 1);
		return ;
	}
	else if (n < 0)
	{
		write(fd, "-", 1);
		num = -n;
	}
	else
		num = n;
	putnbr_to_buffer(num, fd);
}
