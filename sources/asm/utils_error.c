/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:40:26 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/24 17:46:41 by tle-dieu         ###   ########.fr       */
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

int		check_end_str(t_env *e, char *s, int cmd, char c)
{
	char	chr[sizeof(SPACES) + 1];
	int		i;

	i = sizeof(SPACES) - 1;
	chr[i] = c;
	chr[i + 1] = '\0';
	while (i--)
		chr[i] = SPACES[i];
	/* ft_printf("chr: |%s|\n", chr); */
	/* ft_printf("s: |%s|\n", s); */
	s = s + ft_strspn(s, SPACES);
	if (*s == c)
		return (1);
	if (error_header(e, 1, s, cmd) == -1)
		return (-1);
	s = s + ft_strcspn(s, chr);
	while (*(s = s + ft_strspn(s, SPACES)) != c)
	{
		if (error_header(e, 1, s, cmd) == -1)
			return (-1);
		s = s + ft_strcspn(s, chr);
	}
	return (0);
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
