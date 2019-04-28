/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/28 04:55:20 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <errno.h>
#include <stdlib.h>

//verifier que tout est print sur stderr

int		alloc_error(t_env *e)
{
	ft_printf("malloc error\n");
    ft_dprintf(2, COLOR_FATAL(e->tty2));
    ft_dprintf(2, "%s\n", strerror(errno));
	free_lst_file(e);
	exit(EXIT_FAILURE);
	return (-1);
}

int		len_err(char *s)
{
	int len;

	len = ft_strclen(s, '"');
	if (s[len] == '"')
		len += ft_strclen(s + len + 1, '"') + 1;
	else
		len = ft_strcspn(s, "\"\t ");
	return (len + (s[len] == '"'));
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
		ft_dprintf(2, COLOR_LINE(e->tty2), e->actual->name, line->y, extra - line->s + 1);
	if (error == 1)
	{
		ft_dprintf(2, "unexpected expression in %s declaration\n", scmd);
		err_pointer(e->tty2, e->actual->last->s, extra, 0);
		if (ft_strcspn(e->actual->last->s, "\"") + e->actual->last->s > extra)
			err_wave(e->tty2, extra, ft_strcspn(extra, SPACES"\"") - 1);
		else
			err_wave(e->tty2, extra, ft_strcspn(extra, SPACES) - 1);
		ft_printf("\nline: %s\n", extra);
	}
	else if (error == 2)
	{
		ft_dprintf(2, "%s declaration too long (Max length: %d)\n", scmd, cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH);
		err_pointer(e->tty2, e->actual->begin->s, extra++, 0);
		err_wave(e->tty2, extra, ft_strclen(extra, '"') + 1);
	}
	else if (error == 3)
	{
		ft_dprintf(2, "expected string after %s\n", scmd);
		err_pointer(e->tty2, e->actual->begin->s, extra + (*extra == ' ' || *extra == '\t'), !*extra);
	}
	else if (error == 4)
	{
		ft_dprintf(2, "missing terminating '\"' character\n");
		err_pointer(e->tty2, e->actual->begin->s, extra, 0);
	}
	else if (error == 5)
	{
		ft_dprintf(2, "%s already defined\n", scmd);
		err_pointer(e->tty2, e->actual->begin->s, extra++, 0);
		err_wave(e->tty2, extra, len_err(extra));
	}
	else if (error == 6)
	{
		ft_dprintf(2, "invalid command '%.*s'{R}\n", ft_strcspn(extra, SPACES"\""), extra);
		err_pointer(e->tty2, e->actual->begin->s, extra, 0);
		err_wave(e->tty2, extra, ft_strcspn(extra + 1, SPACES"\""));
	}
	if (error)
		ft_dprintf(2, "\n");
	return (!error - (e->actual->error >= 20));
}
