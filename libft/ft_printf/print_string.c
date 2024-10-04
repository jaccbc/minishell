/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:50:22 by vamachad          #+#    #+#             */
/*   Updated: 2024/05/05 00:55:02 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_string(char *s)
{
	int	count;

	if (s == NULL)
		return (write(1, "(null)", 6));
	count = 0;
	while (*s)
	{
		count += write(1, s, 1);
		s++;
	}
	return (count);
}
