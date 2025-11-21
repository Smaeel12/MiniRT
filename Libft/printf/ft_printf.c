/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:15:12 by iboubkri          #+#    #+#             */
/*   Updated: 2025/03/06 15:12:39 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putnbr(unsigned long int n, char *base, unsigned int lbase)
{
	if (n < lbase)
		return (write(1, &base[n % lbase], 1));
	return (ft_putnbr(n / lbase, base, lbase) + write(1, &base[n % lbase], 1));
}

int	ft_putstr(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (write(1, "(null)", 6));
	while (str[i])
		i += write(1, &str[i], 1);
	return (i);
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_placeholders(va_list args, char ps)
{
	unsigned long	addr;
	long int		n;

	if (ps == 'u')
		return (ft_putnbr(va_arg(args, unsigned int), "0123456789", 10));
	else if (ps == 'x')
		return (ft_putnbr(va_arg(args, unsigned int), "0123456789abcdef", 16));
	else if (ps == 'X')
		return (ft_putnbr(va_arg(args, unsigned int), "0123456789ABCDEF", 16));
	if (ps == 'd' || ps == 'i')
	{
		n = va_arg(args, int);
		return (write(1, "-", n < 0) + ft_putnbr((n * ((n < 0) * -1 | 1)),
				"0123456789", 10));
	}
	else if (ps == 'p')
	{
		addr = (unsigned long)va_arg(args, void *);
		if (addr == 0)
			return (write(1, "(nil)", 5));
		return (write(1, "0x", 2) + ft_putnbr(addr, "0123456789abcdef", 16));
	}
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		nc;

	nc = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && (0x01290238 & (1 << (*(format + 1) - ' '))))
		{
			if (*++format == '%')
				nc += (ft_putchar('%'));
			else if (*format == 'c')
				nc += (ft_putchar(va_arg(args, int)));
			else if (*format == 's')
				nc += (ft_putstr(va_arg(args, char *)));
			nc += ft_placeholders(args, *(format));
		}
		else
			nc += ft_putchar(*format);
		format++;
	}
	va_end(args);
	return (nc);
}
