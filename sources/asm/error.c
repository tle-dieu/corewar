/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/18 03:37:41 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <errno.h>
#include <stdlib.h>

//verifier que tout est print sur stderr

int		alloc_error(t_env *e)
{
    ft_dprintf(2, COLOR_FILE(e->tty), e->exname);
    ft_dprintf(2, "%s\n", strerror(errno));
	free_lst_file(e->file);
	exit(EXIT_FAILURE);
	return (-1);
}

int		error_header(t_env *e, int error, char *extra, int cmd)
{
	char	*scmd;
	t_line	*line;

	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	line = error == 1 ? e->actual->last : e->actual->begin;
	if (error)
		++e->actual->error;
	if (error)
		ft_dprintf(2, COLOR_LINE(e->tty), e->actual->name, line->y, extra - line->s + 1);
	if (error == 1)
	{
		ft_dprintf(2, "unexpected expression after %s declaration\n", scmd);
		err_pointer(e->tty, e->actual->last->s, extra);
	}
	else if (error == 2)
	{
		ft_dprintf(2, "%s declaration too long (Max length: %d)\n", scmd, cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH);
		err_pointer(e->tty, e->actual->begin->s, extra++);
		err_wave(e->tty, extra);
	}
	else if (error == 3)
	{
		ft_dprintf(2, "expected string after %s\n", scmd);
		err_pointer(e->tty, e->actual->begin->s, extra);
	}
	else if (error == 4)
	{
		ft_dprintf(2, "missing terminating '\"' character\n");
		err_pointer(e->tty, e->actual->begin->s, extra);
	}
	if (error)
		ft_dprintf(2, "\n");
	return (!error);
}
