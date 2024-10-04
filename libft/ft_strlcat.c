/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 21:21:48 by vamachad          #+#    #+#             */
/*   Updated: 2024/04/22 15:15:07 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dstlen;
	size_t	srclen;

	i = 0;
	j = 0;
	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (dstlen >= size)
		return (size + srclen);
	while (dst[i] && i < size - 1)
		i++;
	while (src[j] && i < size - 1)
		dst[i++] = src[j++];
	dst[i] = 0;
	return (dstlen + srclen);
}

/*int	main(void)
{
	char	s1[] = "Hola Chica guapa";
	char	s2[] = "HolaHola";
	printf("%zu\n", ft_strlcat(s2, s1, 5));
	printf("%s\n", s2);
	return (0);
}*/
