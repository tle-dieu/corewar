/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 10:55:52 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/14 21:04:28 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*ds;
	unsigned char const	*sr;

	ds = (unsigned char *)dest;
	sr = (unsigned char const *)src;
	if (!dest && !src)
		return (NULL);
	while (n--)
		*ds++ = *sr++;
	return (dest);
}
