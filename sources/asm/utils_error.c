/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:40:26 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/17 23:29:00 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>

void	put_strtab(char *s, char replace, int n)
{
	char	buff[100];
	char	*tmp;
	int		i;
	int		j;

	tmp = s;
	while (*s && (s - tmp < n || n == -1))
	{
		i = 0;
		while (i + TAB_SIZE < 100 && *s && (s - tmp < n || n == -1))
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
}

char	*check_end_str(char **end)
{
	char *s;

	ft_printf("check: %s\n", *end);
	s = *end + 1;
	while (*s == ' ' || *s == '\t')
		s++;
	if (!*s)
		return (NULL);
	else
	{
		ft_printf("%hhd: %c\n", *s, *s);
		*end = s;
		return (s);
	}
}

void	err_pointer(int tty, char *s, char *end)
{
	if (tty)
		ft_dprintf(2, "{R}");
	put_strtab(s, 0, -1);
	write(2, "\n", 1);
	put_strtab(s, ' ', end - s);
	if (tty)
		ft_dprintf(2, GREEN_CURS"^{R}");
	else
		ft_dprintf(2, "^");
}

void	err_wave(int tty, char *s)
{
	if (tty)
		ft_dprintf(2, GREEN_CURS);
	put_strtab(s, '~', -1);
	if (tty)
		ft_dprintf(2, "{R}");
}
