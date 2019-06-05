/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assemble.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 05:20:19 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/06/05 17:13:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static void	get_output_file(t_env *e)
{
	char *s;

	if (!(s = ft_strrchr(e->file->name, '.')))
	{
		if (!(e->file->output = ft_strjoin(e->file->name, ".cor")))
			alloc_error(e);
	}
	else
	{
		if (!(e->file->output = ft_strnew(s - e->file->name + 4)))
			alloc_error(e);
		ft_memcpy(e->file->output, e->file->name, s - e->file->name);
		ft_memcpy(e->file->output + (s - e->file->name), ".cor", 5);
	}
}

static void	write_bytecode(t_env *e, unsigned char *header)
{
	int		fd;
	t_file	*file;

	file = e->file;
	if (!file->output)
		get_output_file(e);
	fd = open(file->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1)
	{
		write(fd, header, HEADER_SIZE);
		file->buff = file->begin_buff;
		while (file->buff)
		{
			write(fd, file->buff->s, file->buff->len);
			file->buff = file->buff->next;
		}
		ft_printf("Writing output program to %s\n", file->output);
		close(fd);
	}
	else
	{
		ft_dprintf(2, line_error(ERR_ARGS, e->tty2), e->exname);
		ft_dprintf(2, "%s: '%s'\n", strerror(errno), file->output);
	}
}

static void	assembly_error(t_env *e, unsigned char *header)
{
	if (e->file->error < MAX_ERROR && e->file->i > CHAMP_MAX_SIZE)
		champ_too_big(e);
	if (e->file->error < MAX_ERROR && !(e->file->complete & NAME_CMD))
		missing_cmd(e, header + 4, NAME_CMD);
	if (e->file->error < MAX_ERROR && !(e->file->complete & COMMENT_CMD))
		missing_cmd(e, header + PROG_NAME_LENGTH + 12, COMMENT_CMD);
	if (e->file->warning)
		ft_dprintf(2, "%d %s ", e->file->warning,
			e->file->warning > 1 ? "warnings" : "warning");
	if (e->file->warning && e->file->error)
		ft_dprintf(2, "and ");
	if (e->file->error)
		ft_dprintf(2, "%d %s ", e->file->error,
			e->file->error > 1 ? "errors" : "error");
	if (e->file->error || e->file->warning)
		ft_dprintf(2, "generated.\n");
}

static void	get_bytecode(t_env *e, unsigned char *header)
{
	char	*line;

	while (e->file->error < MAX_ERROR && add_line(e, &line) == 1)
	{
		if (*line == '.')
			get_cmd(e, header + 4, line);
		else if (*line)
		{
			if (!only_label(e, &line))
				parse_inst(e, line);
		}
		if (e->file->last != e->file->begin)
			free_line(&e->file->last, 0);
		else
			e->file->last = NULL;
		free_line(&e->file->begin, 0);
	}
	check_label_call(e);
	if (e->file->error >= MAX_ERROR)
	{
		ft_dprintf(2, line_error(ERR_FATAL, e->tty2));
		ft_dprintf(2, "too many errors emitted, stopping now\n");
	}
}

void		assemble(t_env *e)
{
	unsigned char	*cp;
	unsigned char	header[HEADER_SIZE];
	int				i;

	ft_bzero(header, HEADER_SIZE);
	i = 4;
	cp = header;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_bytecode(e, header);
	i = 4;
	while (i--)
		header[PROG_NAME_LENGTH + 11 - i] = e->file->i >> i * 8;
	assembly_error(e, header);
	if (!e->file->error)
	{
		if (!e->file->error && e->file->options & (O_HEXA | O_BIN))
			dump_bytecode(e, header);
		if (!e->file->error && !(e->file->options & (O_HEXA | O_BIN)))
			write_bytecode(e, header);
	}
}
