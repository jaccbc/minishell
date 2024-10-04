/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vamachad <vamachad@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:17:48 by vamachad          #+#    #+#             */
/*   Updated: 2024/04/22 13:44:36 by vamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s_joined;
	size_t	i;
	size_t	j;

	s_joined = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!s_joined)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		s_joined[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		s_joined[i] = s2[j];
		i++;
		j++;
	}
	s_joined[i] = '\0';
	return (s_joined);
}

/*int	main(void)
{
	char	s1[] = "Hola";
	char	s2[] = " Muchacha";

	printf("%s", ft_strjoin(s1, s2));
}*/
