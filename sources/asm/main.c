/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/03 20:54:50 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "asm.h"
#include <stdlib.h>
#include <fcntl.h>


void	help(char *filename)
{
	ft_printf("Usage: %s <sourcefile.s>\n", filename);
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

int		get_short_option(t_file *option, char *s, char *filename)
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
			help(filename);
		else
			return (assign_option(option, &tmp, NULL, *s));
		s++;
	}
	return (assign_option(option, &tmp, NULL, 0));
}

int		get_long_option(t_file *option, char *s, char *filename)
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
		help(filename);
	else
		return (assign_option(option, &tmp, s, 0));
	return (assign_option(option, &tmp, NULL, 0));
}

int		get_option(t_file *option, char *s, char *filename)
{
	if (!*s || (!*(s + 1) && *s == '-')
	|| !(*s == '-' ? get_long_option(option, s + 1, filename)
	: get_short_option(option, s, filename)))
		return (0);
	return (1);
}

int		error_option(t_file *option, char *filename)
{
	if (option && (option->error || option->file))
	{
		if (option->error)
			ft_printf("%s: illegal option - %c\n", filename, option->error);
		else
			ft_printf("%s: illegal option -- %s\n", filename, option->file);
	}
	ft_printf("Usage: %s <sourcefile.s>\n", filename);
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
				exit(error_option(&option, *av));
			print_option(&option, av[i]);
			option = (t_file){NULL, 0, 0, 0, 0, NULL};
		}
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	t_file *file;

	file = parse_command_line(ac, av);
	if (ac < 2)
		error_option(NULL, *av);
}
