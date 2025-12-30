/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 15:03:36 by iboubkri          #+#    #+#             */
/*   Updated: 2025/12/26 19:06:47 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

double ft_atof(const char *str)
{
	size_t res;
	size_t power_of_10;
	int decimal_seen;
	int sign;

	res = 0;
	sign = 1;
	decimal_seen = 0;
	power_of_10 = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
		sign = (44 - *(str++));
	while (*str) {
		if (*str == '.' && !decimal_seen && *str++)
			decimal_seen = 1;
		if (!(*str >= '0' && *str <= '9'))
			break;
		if (decimal_seen)
			power_of_10 *= 10;
		res = (res << 3) + (res << 1) + (*str++ - '0');
	}
	return (sign * (double)res / (double)power_of_10);
}
