/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/05 15:16:05 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "asm.h"
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>


#define ERR "{bold}{#ff6b6b}"

int		usage(char *ex_name)
{
	ft_printf("usage: %s [-a] [-x] sourcefile.s\n", ex_name);
	ft_printf("       %s [-d] sourcefile.cor\n", ex_name);
	ft_printf("Try `%s (-h | --help)' for more information.\n", ex_name);
	return (0);
}

void	help(char *ex_name)
{
	usage(ex_name);
	exit(0);
}

int		assign_option(t_file *option, t_file *tmp, char *s, char c)
{
	if (c || s)
	{
		if (c)
			option->error = c;
		else
			option->file = s;
		return (0);
	}
	option->annot = tmp->annot | option->annot;
	option->dump = tmp->dump | option->dump;
	option->disas = tmp->disas | option->disas;
	return (1);
}

int		get_short_option(t_file *option, char *s, char *ex_name)
{
	t_file tmp;

	tmp = (t_file){NULL, 0, 0, 0, 0, NULL};
	while (*s)
	{
		if (*s == 'a')
			tmp.annot = 1;
		else if (*s == 'd')
			tmp.disas = 1;
		else if (*s == 'x')
			tmp.dump = 1;
		else if (*s == 'h')
			help(ex_name);
		else
			return (assign_option(option, &tmp, NULL, *s));
		s++;
	}
	return (assign_option(option, &tmp, NULL, 0));
}

int		get_long_option(t_file *option, char *s, char *ex_name)
{
	t_file tmp;

	tmp = (t_file){NULL, 0, 0, 0, 0, NULL};
	if (!ft_strcmp(s, "annotated"))
		tmp.annot = 1;
	else if (!ft_strcmp(s, "dump"))
		tmp.dump = 1;
	else if (!ft_strcmp(s, "disassembly"))
		tmp.disas = 1;
	else if (!ft_strcmp(s, "help"))
		help(ex_name);
	else
		return (assign_option(option, &tmp, s, 0));
	return (assign_option(option, &tmp, NULL, 0));
}

int		get_option(t_file *option, char *s, char *ex_name)
{
	if (!*s || (!*(s + 1) && *s == '-')
			|| !(*s == '-' ? get_long_option(option, s + 1, ex_name)
				: get_short_option(option, s, ex_name)))
		return (0);
	return (1);
}

int		error_file(t_file *option, char *ex_name, char *file)
{
	ft_printf("%s: "ERR"error: {reset}", ex_name);
	if (option->error || option->file)
	{
		if (option->error)
			ft_printf("unknow option - %c\n", option->error);
		else
			ft_printf("unknow option -- %s\n", option->file);
		usage(ex_name);
	}
	else
		ft_printf("%s: '%s'\n", strerror(errno), file);
	return (1);
}

t_file	*parse_command_line(int ac, char **av)
{
	t_file	*file;
	t_file	*last;
	t_file	option;
	int		i;
	int		fd;

	i = 0;
	(void)last;
	(void)file;
	option = (t_file){NULL, 0, 0, 0, 0, NULL};
	while (++i < ac)
	{
		if ((*av[i] != '-' || !get_option(&option, av[i] + 1, *av)))
		{
			if ((fd = open(av[i], O_RDONLY)) == -1)
			{
				exit(error_file(&option, *av, av[i]));
			}
			print_option(&option, av[i]);
			option = (t_file){NULL, 0, 0, 0, 0, NULL};
		}
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	t_file *file;

	if (ac < 2)
		return (usage(av[0]));
	file = parse_command_line(ac, av);
}
