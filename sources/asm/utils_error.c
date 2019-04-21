/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:40:26 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/21 07:23:17 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>

void	put_strtab(char *s, char replace, int n)
{
	char	buff[100];
	int		i;
	int		j;
	char	*t;

	t = s;
	while (*s && (s - t < n || n == -1))
	{
		i = 0;
		while (i + TAB_SIZE < 100 && *s && (s - t < n || n == -1))
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

	s = *end + 1;
	while (*s == ' ' || *s == '\t')
		s++;
	if (!*s)
		return (NULL);
	else
	{
		*end = s;
		return (s);
	}
}

void	err_pointer(int tty, char *s, char *end, int sp)
{
	if (tty)
		ft_dprintf(2, "{R}");
	put_strtab(s, 0, -1);
	if (s != end)
		ft_dprintf(2, "\n%*c", end - s + ft_ncount_occ(s, '\t', end - s) * (TAB_SIZE - 1) + sp, ' ');
	else
		ft_dprintf(2, "\n");
	if (tty)
		ft_dprintf(2, GREEN_CURS"^{R}");
	else
		ft_dprintf(2, "^");
}

void	err_wave(int tty, char *s, int n)
{
	if (tty)
		ft_dprintf(2, GREEN_CURS);
	put_strtab(s, '~', n);
	if (tty)
		ft_dprintf(2, "{R}");
	/* ft_printf("\nextra: |%s| ", s); */
	/* ft_printf("len: %zu\n", n); */
}
