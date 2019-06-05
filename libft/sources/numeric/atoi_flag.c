/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_flag.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 16:20:13 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/31 16:38:29 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_sign(char c, int *sign)
{
	int i;

	i = 0;
	if (c == '-')
	{
		*sign = -1;
		i++;
	}
	else if (c == '+')
		i++;
	return (i);
}

long long	atoi_flag(char **nbr, int *error, int flags, char *stop)
{
	unsigned long long	result;
	int					sign;
	char				*s;

	s = *nbr;
	*error = 0;
	sign = 1;
	result = 0;
	if (!(AT_NOSPACE & flags))
		while (*s == ' ' || (*s >= '\t' && *s <= '\r'))
			s++;
	if (!(flags & AT_NOSIGN))
		s += get_sign(*s, &sign);
	while ((*s >= '0' && *s <= '9')
		&& (!(flags & AT_STOP) || !ft_strchr(stop, *s)))
		result = result * 10 + *s++ - 48;
	if ((AT_NOOVERFLOW & flags && ((!(AT_LONG & flags) && result > 2147483647)
				|| s - *nbr > 19 || result > 9223372036854775807))
		|| (AT_ONLYNUM & flags && (!ft_isdigit(*s) && (!(AT_STOP & flags)
					|| !ft_strchr(stop, *s)))))
		*error = 1;
	if (AT_INCREM & flags && (!(AT_NOERR & flags) || !*error))
		*nbr = s;
	return ((AT_LONG & flags ? (long long)result : (int)result) * sign);
}
