/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:32:50 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/09 13:12:46 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static t_file	*add_file(t_file **file, t_file *last, char *name, t_file *opt)
{
	t_file *new;

	if (!(new = (t_file *)malloc(sizeof(t_file))))
	{
		free_lst_file(*file);
		return (NULL);
	}
	new->name = name;
	new->fd = opt->fd;
	new->annot = opt->annot;
	new->disas = opt->disas;
	new->dump = opt->dump;
	new->error = 0;
	new->begin = NULL;
	new->last = NULL;
	new->next = NULL;
	if (*file)
		last->next = new;
	else
		*file = new;
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

	tmp = (t_file){NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL};
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

	tmp = (t_file){NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL};
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

t_file			*parse_command_line(int ac, char **av)
{
	t_file	*file;
	t_file	*last;
	t_file	option;
	int		i;
	char	*s;

	i = 0;
	file = NULL;
	option = (t_file){NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL};
	while (++i < ac)
	{
		s = av[i];
		if (*s != '-' || !*++s || (*s == '-' && !*(s + 1))
		|| !(*s == '-' ? get_long_option(&option, s + 1, av[0])
		: get_short_option(&option, s, av[0])))
		{
			if ((option.fd = open(av[i], O_RDONLY)) == -1
			|| read(option.fd, av[i], 0) < 0)
				exit(error_file(&option, *av, av[i], file));
			if (!(last = add_file(&file, last, av[i], &option)))
				return (alloc_error(av[0]));
			option = (t_file){NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL};
		}
	}
	return (file);
}
