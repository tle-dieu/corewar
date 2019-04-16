/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:32:50 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/16 23:28:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static t_file	*add_file(t_env *e, char *name, t_file *opt)
{
	t_file *new;

	if (!(new = (t_file *)malloc(sizeof(t_file))))
		alloc_error(e);
	new->name = name;
	new->fd = opt->fd;
	new->annot = opt->annot;
	new->disas = opt->disas;
	new->dump = opt->dump;
	new->error = 0;
	new->begin = NULL;
	new->last = NULL;
	new->next = NULL;
	if (e->file)
		e->actual->next = new;
	else
		e->file = new;
	return (new);
}

static int		assign_option(t_file *option, t_file *tmp, char *s, char c)
{
	if (c || s)
	{
		if (c)
			option->error = c;
		else
			option->name = s;
		return (0);
	}
	option->annot = tmp->annot | option->annot;
	option->dump = tmp->dump | option->dump;
	option->disas = tmp->disas | option->disas;
	return (1);
}

static int		get_short_option(t_file *option, char *s, char *ex_name)
{
	t_file tmp;

	tmp = (t_file){NULL, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL};
	while (*s)
	{
		if (*s == 'a')
			tmp.annot = 1;
		else if (*s == 'd')
			tmp.disas = 1;
		else if (*s == 'x')
			tmp.dump = 1;
		else if (*s == 'h')
			usage(ex_name, 1);
		else
			return (assign_option(option, &tmp, NULL, *s));
		s++;
	}
	return (assign_option(option, &tmp, NULL, 0));
}

static int		get_long_option(t_file *option, char *s, char *ex_name)
{
	t_file tmp;

	tmp = (t_file){NULL, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL};
	if (!ft_strcmp(s, "annotated"))
		tmp.annot = 1;
	else if (!ft_strcmp(s, "dump"))
		tmp.dump = 1;
	else if (!ft_strcmp(s, "disassembly"))
		tmp.disas = 1;
	else if (!ft_strcmp(s, "help"))
		usage(ex_name, 1);
	else
		return (assign_option(option, &tmp, s, 0));
	return (assign_option(option, &tmp, NULL, 0));
}

int		parse_command_line(t_env *e, int ac, char **av)
{
	t_file	option;
	int		i;
	char	*s;

	i = 0;
	option = (t_file){NULL, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL};
	while (++i < ac)
	{
		s = av[i];
		if (*s != '-' || !*++s || (*s == '-' && !*(s + 1))
		|| !(*s == '-' ? get_long_option(&option, s + 1, e->exname)
		: get_short_option(&option, s, e->exname)))
		{
			if ((option.fd = open(av[i], O_RDONLY)) == -1
			|| read(option.fd, av[i], 0) < 0)
				return (error_file(&option, e->exname, av[i], e->file));
			e->actual = add_file(e, av[i], &option);
			option = (t_file){NULL, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL};
		}
	}
	return (1);
}
