/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:06:48 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/06 15:11:18 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	count_len(int n)
{
	if (!n)
		return (0);
	return (1 + count_len(n / 10));
}

static void	putnbr_to_buffer(unsigned int n, char *str)
{
	if (!n)
		return ;
	*str = (n % 10) + '0';
	putnbr_to_buffer(n / 10, str - 1);
	return ;
}

char	*ft_itoa(int n)
{
	size_t			len;
	unsigned int	num;
	char			*str;

	len = (n <= 0) + count_len(n);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	num = n * ((n < 0) * -1 | 1);
	ft_memset(str, '0', len + 1);
	str[len--] = '\0';
	putnbr_to_buffer(num, str + len);
	if (n < 0)
		str[0] = '-';
	return (str);
}
