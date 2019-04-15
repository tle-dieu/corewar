/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/14 21:01:02 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/14 21:19:58 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memjoin(void const *s1, void const *s2, size_t n1, size_t n2)
{
	void				*new;
	unsigned char const	*src;
	unsigned char		*dest;

	src = (const unsigned char *)s1;
	if (!(new = malloc(n1 + n2)))
		return (NULL);
	dest = (unsigned char *)new;
	while (n1--)
		*dest++ = *src++;
	src = (const unsigned char *)s2;
	while (n2--)
		*dest++ = *src++;
	return (new);
}
