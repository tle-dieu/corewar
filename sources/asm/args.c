/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:32:50 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/18 12:58:08 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static void		init_file(t_file *file)
{
	file->output = NULL;
	file->warning = 0;
	file->buff = NULL;
	file->begin_buff = NULL;
	file->line_nb = 1;
	file->output = NULL;
	file->champ_part = 0;
	file->label = NULL;
	file->i = 0;
	file->error = 0;
	file->complete = 0;
	file->begin = NULL;
	file->last = NULL;
	file->next = NULL;
}

static t_file	*add_file(t_env *e, char *name, unsigned options, int fd)
{
	t_file *new;

	if (!(new = (t_file *)malloc(sizeof(t_file))))
		alloc_error(e);
	init_file(new);
	new->options = options;
	new->name = name;
	new->fd = fd;
	if ((e->output && !(new->output = ft_strdup(e->output))))
	{
		free(new);
		alloc_error(e);
	}
	e->output = NULL;
	if (e->file)
		e->actual->next = new;
	else
		e->file = new;
	return (new);
}

static int		valid_file(int fd, unsigned *options)
{
	char	buff[1];
	off_t	size;
	off_t	current_pos;
	int		ret;

	if ((ret = read(fd, buff, 0)) < 0)
		return (0);
	if ((current_pos = lseek(fd, 0, SEEK_CUR)) == -1)
		return (0);
	if ((size = lseek(fd, 0, SEEK_END)) == -1)
		return (0);
	if ((ret = read(fd, buff, 1)) < 0)
		return (0);
	if (!size && ret)
	{
		*options |= O_INVALID_FILE_ERR;
		return (0);
	}
	if ((lseek(fd, current_pos, SEEK_SET) == -1))
		return (0);
	return (1);
}

static int		check_args(t_env *e, char **arg, unsigned *options)
{
	char	*s;
	int		ret;

	s = *arg;
	if (*s != '-' || !*++s || (*s == '-' && !*(s + 1)))
	{
		*arg = s;
		return (0);
	}
	ret = 0;
	if (*s == '-')
		ret = get_long_option(e, options, &s);
	else
		ret = get_short_option(e, options, &s);
	*arg = s;
	return ((*options & O_OUTPUT) || ret);
}

int				parse_command_line(t_env *e, int ac, char **av)
{
	unsigned	options;
	int			fd;
	char		*arg;

	options = 0;
	while (++e->i < ac)
	{
		arg = av[e->i];
		if (!check_args(e, &arg, &options))
		{
			if (options & O_OUTPUT_ERR
				|| (fd = open(av[e->i], O_RDONLY)) == -1
				|| !valid_file(fd, &options))
				return (error_file(e, arg, av[e->i], options));
			e->actual = add_file(e, av[e->i], options, fd);
			options = 0;
		}
		if (O_OUTPUT & options)
			if (!output_file(e, ac, av, &options))
				return (0);
	}
	return (1);
}
