/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 20:26:52 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/25 19:02:24 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

size_t	ft_strspn(const char *s, const char *accept)
{
	const char	*tmp;
	size_t		i;

	i = 0;
	while (s[i])
	{
		tmp = accept;
		while (*tmp != s[i])
			if (!*tmp++)
				return (i);
		i++;
	}
	return (i);
}
