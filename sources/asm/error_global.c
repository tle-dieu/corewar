/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 00:08:56 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 00:17:29 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>
#include <errno.h>

void	alloc_error(t_env *e)
{
	ft_dprintf(2, line_error(ERR_FATAL, e->tty2));
	ft_dprintf(2, "%s\n", strerror(errno));
	free_lst_file(e);
	exit(-1);
}

int		basic_error(t_env *e, char *str, char *err_string, int wave)
{
	++e->file->error;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, e->file->last->y, str - e->file->last->s + 1);
	ft_dprintf(2, err_string);
	err_pointer(e->tty2, e->file->last->s, str);
	if (wave)
		err_wave(e->tty2, str, wave);
	ft_dprintf(2, "\n");
	return (1);
}

void	champ_too_big(t_env *e)
{
	++e->file->warning;
	ft_dprintf(2, line_error(WARNING_FILE, e->tty2), e->file->name);
	ft_dprintf(2, "bytecode of the champion too big for the vm ");
	ft_dprintf(2, "(%d for %d bytes)\n", e->file->i, CHAMP_MAX_SIZE);
	if (e->tty2)
		ft_dprintf(2, "{R}");
}

void	invalid_cmd(t_env *e, char *error, int cmd)
{
	t_line	*line;
	char	*scmd;

	++e->file->warning;
	line = e->file->begin;
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "invalid command '%.*s'{R}\n",
		ft_strcspn(error, SPACES"\""), error);
	err_pointer(e->tty2, e->file->begin->s, error);
	err_wave(e->tty2, error, ft_strcspn(error + 1, SPACES"\""));
	ft_dprintf(2, "\n");
}
