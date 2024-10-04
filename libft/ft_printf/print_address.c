/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_address.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:53:10 by vamachad          #+#    #+#             */
/*   Updated: 2024/05/05 02:10:31 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_digits(unsigned long long n)
{
	int	digits;

	if (!n)
		return (1);
	digits = 0;
	while (n != 0)
	{
		n /= 16;
		digits++;
	}
	return (digits);
}

static void	print_ptr(unsigned long long n)
{
	if (n >= 16)
		print_ptr(n / 16);
	write(1, &"0123456789abcdef"[n % 16], 1);
}

int	print_address(void *ptr)
{
	if (!ptr)
		return (write(1, "(nil)", 5));
	write(1, "0x", 2);
	print_ptr((unsigned long long)ptr);
	return (count_digits((unsigned long long)ptr) + 2);
}
