/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:58:05 by vamachad          #+#    #+#             */
/*   Updated: 2024/05/06 16:31:03 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_unsigned_digits(unsigned long n, unsigned int base)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= base;
		count++;
	}
	return (count);
}

int	print_unsigned(unsigned int n, unsigned int base, bool caps)
{
	unsigned long	nbr;

	nbr = n;
	if (nbr >= base)
		print_unsigned(nbr / base, base, caps);
	if (caps == true)
		write(1, &"0123456789ABCDEF"[nbr % base], 1);
	if (caps == false)
		write(1, &"0123456789abcdef"[nbr % base], 1);
	return (count_unsigned_digits((unsigned long)n, base));
}
