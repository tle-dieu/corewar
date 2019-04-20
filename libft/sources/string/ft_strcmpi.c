/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmpi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 20:24:19 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/20 20:40:16 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int		ft_strcmpi(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]
	|| (s1[i] >= 'a' && s1[i] <= 'z' && s1[i] - 32 == s2[i])
	|| (s1[i] >= 'A' && s1[i] <= 'Z' && s1[i] + 32 == s2[i])))
		i++;
	return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
}
