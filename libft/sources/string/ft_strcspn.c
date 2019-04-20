/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 20:26:16 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/20 20:26:28 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

size_t	ft_strcspn(const char *s, const char *reject)
{
	const char	*tmp;
	size_t		i;

	i = 0;
	while (s[i])
	{
		tmp = reject;
		while (*tmp)
			if (*tmp++ == s[i])
				return (i);
		i++;
	}
	return (i);
}
