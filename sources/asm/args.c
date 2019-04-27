/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:32:50 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/27 16:48:21 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static t_file	*add_file(t_env *e, char *name, unsigned options, int fd)
{
	t_file *new;

	if (!(new = (t_file *)malloc(sizeof(t_file))))
		alloc_error(e);
	new->line_nb = 1;
	new->nb_inst = 0;
	new->output = NULL;
	new->name = name;
	new->label = NULL;
	new->fd = fd;
	new->i = 0;
	new->output = e->output ? ft_strdup(e->output) : NULL;
	e->output = NULL;
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

int		color_option(t_env *e, char **line)
{
	int tmp;
	size_t len;
	char	*s;

	tmp = -1;
	(*line) += 5;
	if (**line == '=')
	{
		s = ++(*line);
		len = ft_strlen(s);
		if (!ft_strncmp(s, "always", len) || !ft_strncmp(s, "yes", len) || !ft_strncmp(s, "force", len))
			tmp = 1;	
		else if (!ft_strncmp(s, "never", len) || !ft_strncmp(s, "no", len) || !ft_strncmp(s, "none", len))
			tmp = 0;
		if (!ft_strncmp(s, "auto", len) || !ft_strncmp(s, "tty", len) || !ft_strncmp(s, "if-tty", len))
			tmp = tmp != -1 ? 2 : e->tty;
		if (tmp == 2)
			return (O_COL_AMBIGUOUS_ERR);
		if (tmp == -1)
			return (O_COL_INVALID_ERR);
		e->tty = tmp;
	}
	return (0);
}

static int		output_file(t_env *e, char **av)
{
	if (e->i + 1 >= e->ac)
		return (O_OUTPUT_ERR);
	e->output = av[++e->i];
	return (O_OUTPUT);
}

static int		get_short_option(t_env *e, unsigned *options, char **s, char **av)
{
	unsigned tmp;

	tmp = 0;
	while (**s)
	{
		if (**s == 'a')
			tmp |= O_ANNOT;
		else if (**s == 'd')
			tmp |= O_DISAS;
		else if (**s == 'x')
			tmp |= O_DUMP;
		else if (**s == 'o')
			tmp |= tmp & O_OUTPUT ? 0 : output_file(e, av);
		else if (**s == 'h')
			usage(e, 1);
		else if (!(tmp & O_OUTPUT))
			tmp |= O_SHORT_ERR;
		if ((*options |= tmp & ~0xff))
			return (0);
		(*s)++;
	}
	return (*options |= tmp);
}

static int		get_long_option(t_env *e, unsigned *options, char **s, char **av)
{
	if (!ft_strcmp(++(*s), "annotated"))
		*options |= O_ANNOT;
	else if (!ft_strcmp(*s, "dump"))
		*options |= O_DUMP;
	else if (!ft_strcmp(*s, "disassembly"))
		*options |= O_DISAS;
	else if (!ft_strncmp(*s, "color", 5)
	&& (!*((*s) + 5) || *((*s) + 5) == '='))
		return (!(*options |= color_option(e, s)));
	else if (!ft_strcmp(*s, "output"))
		*options |= output_file(e, av);
	else if (!ft_strcmp(*s, "help"))
		usage(e, 1);
	else
		*options |= O_LONG_ERR;
	return (!(~0xff & *options));
}

int		parse_command_line(t_env *e, int ac, char **av)
{
	unsigned options;
	int		fd;
	char	*s;

	options = 0;
	while (++e->i < ac)
	{
		s = av[e->i];
		if (*s != '-' || !*++s || (*s == '-' && !*(s + 1))
		|| !(*s == '-' ? get_long_option(e, &options, &s, av)
		: get_short_option(e, &options, &s, av)))
		{
			if (e->i >= ac || (fd = open(av[e->i], O_RDONLY)) == -1
			|| read(fd, av[e->i], 0) < 0)
				return (error_file(e, s, av[e->i], options));
			e->actual = add_file(e, av[e->i], options, fd);
			options = 0;
		}
	}
	return (1);
}
