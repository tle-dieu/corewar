/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:40:26 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/13 17:32:54 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>

void	put_strtab(char *s, char replace)
{
	char	buff[100];
	int		i;
	int		j;

	while (*s)
	{
		i = 0;
		while (i + TAB_SIZE < 100 && *s)
		{
			if (*s == '\t')
			{
				j = 0;
				while (j++ < TAB_SIZE)
					buff[i++] = replace ? replace : ' ';
			}
			else
				buff[i++] = !replace ? *s : replace;
			s++;
		}
		write(2, buff, i);
	}
	write(2, "\n", 1);
}

char	*check_end_str(char **end)
{
	char *s;

	s = *end + 1;
	while (*s == ' ' || *s == '\t')
		s++;
	if (!*s)
		return (NULL);
	else
	{
		ft_printf("%hhd\n", *s);
		*end = s;
		return (s);
	}
}

void	err_pointer(char *s, char *end)
{
	put_strtab(s, 0);
	ft_dprintf(2, "\x1b[%dC", end - s + ft_ncount_occ(s, '\t', end - s) * (TAB_SIZE - 1));
	ft_dprintf(2, GREEN_CURS"%c{R}", '^');
}

void	err_wave(char *s)
{
	ft_dprintf(2, GREEN_CURS);
	put_strtab(s, '~');
	ft_dprintf(2, "{R}");
}
