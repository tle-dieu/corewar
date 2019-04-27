/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 22:08:29 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/27 22:10:50 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strnjoin(char const *s1, char const *s2, size_t n1, size_t n2)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (*s1 && n1--)
		str[i++] = *s1++;
	while (*s2 && n2--)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}
