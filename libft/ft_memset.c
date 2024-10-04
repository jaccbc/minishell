/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 20:18:57 by vamachad          #+#    #+#             */
/*   Updated: 2024/05/01 17:36:20 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	void	*ptr;

	ptr = str;
	while (n--)
		*((unsigned char *)ptr++) = (unsigned char)c;
	return (str);
}

/*int	main(void)
{
	char	s2[] = "";
	char	s1[] = "Hola Chico";

	ft_memset(s2, 'o', 0);
	printf("%s\n", s2);
	printf("%s\n", s1);
	ft_memset(s1 + 3, '*', 5);
	printf("%s\n", s1);
	return (0);
}*/
