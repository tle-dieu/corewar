/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_sep.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 01:38:12 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/18 01:38:19 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin_sep(char const *s1, char const *s2, char c)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!str)
		return (NULL);
	while (*s1)
		str[i++] = *s1++;
	str[i++] = c;
	while (*s2)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}
