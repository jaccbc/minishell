/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 18:59:55 by vamachad          #+#    #+#             */
/*   Updated: 2024/05/05 00:47:47 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_format(char specifier, va_list ap)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count += print_char(va_arg(ap, int));
	else if (specifier == 's')
		count += print_string(va_arg(ap, char *));
	else if (specifier == 'p')
		count += print_address(va_arg(ap, void *));
	else if (specifier == 'd' || specifier == 'i')
		count += print_nbr(va_arg(ap, int));
	else if (specifier == 'u')
		count += print_unsigned(va_arg(ap, unsigned int), 10, false);
	else if (specifier == 'x')
		count += print_unsigned(va_arg(ap, unsigned int), 16, false);
	else if (specifier == 'X')
		count += print_unsigned(va_arg(ap, unsigned int), 16, true);
	else if (specifier == '%')
		count += write(1, "%", 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	ap;

	if (!format)
		return (-1);
	count = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%')
			count += print_format(*(++format), ap);
		else
			count += write(1, format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}

/* int	main(void)
{
	int my = ft_printf("%u\n", -1);
	ft_printf("%d\n", my);
	int not = printf("%u\n", -1);
	printf("%d\n", not);
} */
