/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 01:09:26 by vamachad          #+#    #+#             */
/*   Updated: 2024/04/30 20:10:24 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*d;

	d = dest;
	if (!dest && !src)
		return (NULL);
	if (src < dest)
	{
		src += n - 1;
		dest += n - 1;
		while (n--)
			*(unsigned char *)dest-- = *(unsigned char *)src--;
	}
	else
	{
		while (n--)
			*(unsigned char *)dest++ = *(unsigned char *)src++;
	}
	return (d);
}

/*int main(void)
{
	char s1[] = "HOLA";
	char s2[7];
	ft_memmove(s2, s1, 3);
	printf("%s", s2);
}*/
