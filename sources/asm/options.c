/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 21:49:13 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/08 15:22:29 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static int	color_option(t_env *e, char **line)
{
	int		tmp;
	size_t	len;
	char	*s;

	tmp = -1;
	if (*((*line) += 5) == '=')
	{
		s = ++(*line);
		if (!ft_strncmp(s, "always", (len = ft_strlen(s)))
				|| !ft_strncmp(s, "yes", len) || !ft_strncmp(s, "force", len))
			tmp = 1;
		else if (!ft_strncmp(s, "never", len) || !ft_strncmp(s, "no", len)
				|| !ft_strncmp(s, "none", len))
			tmp = 0;
		if (!ft_strncmp(s, "auto", len) || !ft_strncmp(s, "tty", len)
				|| !ft_strncmp(s, "if-tty", len))
			tmp = tmp != -1 ? 2 : e->tty2;
		if (tmp == 2)
			return (O_COL_AMBIGUOUS_ERR);
		if (tmp == -1)
			return (O_COL_INVALID_ERR);
		e->tty1 = tmp;
		e->tty2 = tmp;
	}
	return (0);
}

int			output_file(t_env *e, int ac, char **av, unsigned *options)
{
	*options &= ~O_OUTPUT;
	if (++e->i >= ac)
	{
		*options |= O_OUTPUT_ERR;
		return (error_file(e, NULL, av[e->i], *options));
	}
	else if (*av[e->i])
		e->output = av[e->i];
	return (1);
}

int			get_short_option(t_env *e, unsigned *options, char **s)
{
	unsigned tmp;

	tmp = 0;
	while (**s)
	{
		if (**s == 'x')
			tmp |= O_HEXA;
		else if (**s == 'b')
			tmp |= O_BIN;
		else if (**s == 'l')
			tmp |= O_LONG;
		else if (**s == 'd')
			tmp |= O_DISAS;
		else if (**s == 'o')
			tmp |= O_OUTPUT;
		else if (**s == 'h')
			help(e);
		else
			tmp |= O_SHORT_ERR;
		if ((*options |= tmp & ~0xff) & ~0xff)
			return (0);
		(*s)++;
	}
	return (*options |= tmp);
}

int			get_long_option(t_env *e, unsigned *options, char **s)
{
	if (!ft_strcmp(++(*s), "hexadecimal"))
		*options |= O_HEXA;
	else if (!ft_strcmp(*s, "binary"))
		*options |= O_BIN;
	else if (!ft_strcmp(*s, "long-dump"))
		*options |= O_LONG;
	else if (!ft_strcmp(*s, "disassembly"))
		*options |= O_DISAS;
	else if (!ft_strncmp(*s, "color", 5)
			&& (!*((*s) + 5) || *((*s) + 5) == '='))
		return (!(*options |= color_option(e, s)));
	else if (!ft_strcmp(*s, "output"))
		*options |= O_OUTPUT;
	else if (!ft_strcmp(*s, "help"))
		help(e);
	else
		*options |= O_LONG_ERR;
	return (!(~0xff & *options));
}
