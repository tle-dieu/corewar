/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/30 17:38:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <errno.h>
#include <stdlib.h>

//verifier que tout est print sur stderr

int		alloc_error(t_env *e)
{
	ft_dprintf(2, line_error(ERR_FATAL, e->tty2));
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
		len = ft_strcspn(s, SPACES"\"");
	return (len + (s[len] == '"'));
}

void	basic_error(t_env *e, char *str, char *err_string, int wave)
{
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, e->file->last->y , str - e->file->last->s);
	ft_dprintf(2, err_string);
	err_pointer(e->tty2, e->file->last->s, str, 0);
	if (wave)
		err_wave(e->tty2, str, wave);
	ft_dprintf(2, "\n");
}

int		error_header(t_env *e, int error, char *extra, int cmd)
{
	char	*scmd;
	t_line	*line;

	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	line = error == 1 ? e->file->last : e->file->begin;
	if (error == 6)
		++e->file->warning;
	else if (error)
		++e->file->error;
	if (error)
		ft_dprintf(2, line_error(error != 6 ? ERR_LINE : WARNING_LINE, e->tty2), e->file->name, line->y, extra - line->s + 1);
	if (error == 1)
	{
		ft_dprintf(2, "unexpected expression in %s declaration\n", scmd);
		err_pointer(e->tty2, e->file->last->s, extra, 0);
		if (ft_strcspn(e->file->last->s, "\"") + e->file->last->s > extra)
			err_wave(e->tty2, extra, ft_strcspn(extra, SPACES"\"") - 1);
		else
			err_wave(e->tty2, extra, ft_strcspn(extra, SPACES) - 1);
		ft_printf("\nline: %s\n", extra);
	}
	else if (error == 2)
	{
		ft_dprintf(2, "%s declaration too long (Max length: %d)\n", scmd, cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH);
		err_pointer(e->tty2, e->file->begin->s, extra++, 0);
		err_wave(e->tty2, extra, ft_strclen(extra, '"') + 1);
	}
	else if (error == 3)
	{
		ft_dprintf(2, "expected string after %s\n", scmd);
		err_pointer(e->tty2, e->file->begin->s, extra + (ft_strchr(SPACES, *extra) != NULL), !*extra);
	}
	else if (error == 4)
	{
		ft_dprintf(2, "missing terminating '\"' character\n");
		err_pointer(e->tty2, e->file->begin->s, extra, 0);
	}
	else if (error == 5)
	{
		ft_dprintf(2, "%s already defined (ignored)\n", scmd); // passer en warning
		err_pointer(e->tty2, e->file->begin->s, extra++, 0);
		err_wave(e->tty2, extra, len_err(extra));
	}
	else if (error == 6)
	{
		ft_dprintf(2, "invalid command '%.*s'{R}\n", ft_strcspn(extra, SPACES"\""), extra);
		err_pointer(e->tty2, e->file->begin->s, extra, 0);
		err_wave(e->tty2, extra, ft_strcspn(extra + 1, SPACES"\""));
	}
	if (error)
		ft_dprintf(2, "\n");
	return (!error - (e->file->error >= 20));
}
