/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:32:50 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/18 00:21:19 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static t_file	*add_file(t_env *e, char *name, unsigned options, int fd)
{
	t_file *new;

	if (!(new = (t_file *)malloc(sizeof(t_file))))
		alloc_error(e);
	new->name = name;
	new->fd = fd;
	new->options = options;
	new->error = 0;
	new->complete = 0;
	new->begin = NULL;
	new->last = NULL;
	new->next = NULL;
	if (e->file)
		e->actual->next = new;
	else
		e->file = new;
	return (new);
}

static int		get_short_option(t_env *e, unsigned *options, char **s)
{
	unsigned tmp_opt;

	tmp_opt = 0;
	while (**s)
	{
		if (**s == 'a')
			tmp_opt |= O_ANNOT;
		else if (**s == 'd')
			tmp_opt |= O_DISAS;
		else if (**s == 'x')
			tmp_opt |= O_DUMP;
		else if (**s == 'h')
			usage(e, 1);
		else
		{
			*options |= O_SHORT_ERR;
			return (0);
		}
		(*s)++;
	}
	*options |= tmp_opt;
	return (1);
}

static int		get_long_option(t_env *e, unsigned *options, char **s)
{
	(*s)++;
	if (!ft_strcmp(*s, "annotated"))
		*options |= O_ANNOT;
	else if (!ft_strcmp(*s, "dump"))
		*options |= O_DUMP;
	else if (!ft_strcmp(*s, "disassembly"))
		*options |= O_DISAS;
	else if (!ft_strcmp(*s, "no-color"))
		e->tty = 0;
	else if (!ft_strcmp(*s, "help"))
		usage(e, 1);
	else
	{
		*options |= O_LONG_ERR;
		return (0);
	}
	return (1);
}

int		parse_command_line(t_env *e, int ac, char **av)
{
	unsigned options;
	int		i;
	int		fd;
	char	*s;

	i = 0;
	options = 0;
	while (++i < ac)
	{
		s = av[i];
		if (*s != '-' || !*++s || (*s == '-' && !*(s + 1))
		|| !(*s == '-' ? get_long_option(e, &options, &s)
		: get_short_option(e, &options, &s)))
		{
			if ((fd = open(av[i], O_RDONLY)) == -1 || read(fd, av[i], 0) < 0)
				return (error_file(e, s, av[i], options));
			e->actual = add_file(e, av[i], options, fd);
			options = 0;
		}
	}
	return (1);
}
