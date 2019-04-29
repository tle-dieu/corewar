/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:32:50 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/30 00:28:31 by tle-dieu         ###   ########.fr       */
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
	new->warning = 0;
	new->line_nb = 1;
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

static int		get_short_option(t_env *e, unsigned *options, char **s)
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
			tmp |= O_LONG; // changer en short ou pas
		else if (**s == 'd')
			tmp |= O_DISAS;
		else if (**s == 'o')
			tmp |= O_OUTPUT;
		else if (**s == 'h')
			usage(e, 1);
		else
			tmp |= O_SHORT_ERR;
		if ((*options |= tmp & ~0xff) & ~0xff)
			return (0);
		(*s)++;
	}
	return (*options |= tmp);
}

static int		get_long_option(t_env *e, unsigned *options, char **s)
{
	if (!ft_strcmp(++(*s), "hexa"))
		*options |= O_HEXA;
	else if (!ft_strcmp(*s, "binary"))
		*options |= O_BIN;
	else if (!ft_strcmp(*s, "long-print"))
		*options |= O_LONG;
	else if (!ft_strcmp(*s, "disassembly"))
		*options |= O_DISAS;
	else if (!ft_strncmp(*s, "color", 5)
	&& (!*((*s) + 5) || *((*s) + 5) == '='))
		return (!(*options |= color_option(e, s)));
	else if (!ft_strcmp(*s, "output"))
		*options |= O_OUTPUT;
	else if (!ft_strcmp(*s, "help"))
		usage(e, 1);
	else
		*options |= O_LONG_ERR;
	return (!(~0xff & *options));
}

int		valid_file(int fd, unsigned *options)
{
	char	buff[1];
	off_t	size;
	
	size = lseek(fd, 0, SEEK_END);
	if (size > MAX_FILE_SIZE || (!size && read(fd, buff, 1)))
	{
		*options |= O_INVALID_FILE_ERR;
		close(fd);
		return (0);
	}
	return (1);
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
		if ((*s != '-' || !*++s || (*s == '-' && !*(s + 1))
		|| !(*s == '-' ? get_long_option(e, &options, &s)
		: get_short_option(e, &options, &s))) && (!(options & O_OUTPUT)))
		{
			if (options & O_OUTPUT_ERR
			|| (fd = open(av[e->i], O_RDONLY)) == -1
			|| read(fd, av[e->i], 0) < 0 || !valid_file(fd, &options))
				return (error_file(e, s, av[e->i], options));
			e->actual = add_file(e, av[e->i], options, fd);
			options = 0;
		}
		if (O_OUTPUT & options)
		{
			options &= ~O_OUTPUT;
			if (++e->i >= ac)
			{
				options |= O_OUTPUT_ERR;
				return (error_file(e, s, av[e->i], options)); // pas d'utiliser error file
			}
			else if (*av[e->i])
				e->output = av[e->i];
		}
	}
	return (1);
}
