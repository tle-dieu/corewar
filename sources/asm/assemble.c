/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assemble.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 05:20:19 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 02:17:57 by tle-dieu         ###   ########.fr       */
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
		if (!(e->file->output = ft_strnew(s - e->file->name + 4))
				|| !ft_memcpy(e->file->output, e->file->name, s - e->file->name)
				|| !ft_memcpy(e->file->output + (s - e->file->name), ".cor", 5))
			alloc_error(e);
	}
}

static void	write_bytecode(t_env *e, unsigned char *header)
{
	int		fd;

	if (!e->file->output)
		get_output_file(e);
	fd = open(e->file->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1)
	{
		write(fd, header, HEADER_SIZE);
		e->file->buff = e->file->begin_buff;
		while (e->file->buff)
		{
			write(fd, e->file->buff->s, e->file->buff->len);
			e->file->buff = e->file->buff->next;
		}
		ft_printf("Writing output program to %s\n", e->file->output);
		close(fd);
	}
	else
	{
		ft_dprintf(2, line_error(ERR_ARGS, e->tty2), e->exname);
		ft_dprintf(2, "%s: '%s'\n", strerror(errno), e->file->output);
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
