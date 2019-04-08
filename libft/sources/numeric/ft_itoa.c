/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 11:22:27 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/08 11:29:53 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char			*ft_itoa(int n)
{
	char			nbr[11];
	unsigned int	abs;
	char			*res;
	char			*dp;
	char			*cp;

	abs = n < 0 ? -n : n;
	dp = nbr;
	while (abs)
	{
		*dp++ = abs % 10 + 48;
		abs /= 10;
	}
	if (n <= 0)
		*dp++ = n ? '-' : '0';
	if (!(res = (char *)malloc(sizeof(char) * (dp - nbr + 1))))
		return (NULL);
	cp = res;
	while (dp != nbr)
		*cp++ = *--dp;
	*cp = '\0';
	return (res);
}
