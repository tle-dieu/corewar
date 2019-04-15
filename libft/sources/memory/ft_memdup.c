/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/14 17:48:03 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/15 01:55:53 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memdup(const void *src, size_t n)
{
	void 				*new;
	unsigned char		*ds;
	unsigned char const	*sr;

	if (!src || !(new = malloc(n)))
		return (NULL);
	ds = (unsigned char *)new;
	sr = (unsigned char const *)src;
	while (n--)
		*ds++ = *sr++;
	return (new);
}
