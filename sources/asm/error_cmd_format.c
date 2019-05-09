/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_cmd_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 00:30:56 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 17:43:56 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		expect_str(t_env *e, char *error, int cmd)
{
	t_line	*line;
	char	*scmd;

	++e->file->error;
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	line = e->file->begin;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 2);
	ft_dprintf(2, "expected string after %s\n", scmd);
	err_pointer(e->tty2, line->s, error + 1 - (!ft_strchr(SPACES, *error)));
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		cmd_too_long(t_env *e, char *error, int cmd)
{
	t_line	*line;
	char	*scmd;
	int		max;

	++e->file->error;
	max = cmd == COMMENT_CMD ? COMMENT_LENGTH : PROG_NAME_LENGTH;
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	line = e->file->begin;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "%s declaration too long (Max length: %d)\n", scmd, max);
	err_pointer(e->tty2, line->s, error++);
	err_wave(e->tty2, error, ft_strclen(error, '"') + 1);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		unexpected_expression(t_env *e, char *error, int cmd)
{
	char	*scmd;
	t_line	*line;

	++e->file->error;
	line = e->file->last;
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "unexpected expression in %s declaration\n", scmd);
	err_pointer(e->tty2, e->file->last->s, error);
	if (ft_strcspn(e->file->last->s, "\"") + e->file->last->s > error)
		err_wave(e->tty2, error, ft_strcspn(error, SPACES"\"") - 1);
	else
		err_wave(e->tty2, error, ft_strcspn(error, SPACES) - 1);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		missing_quote(t_env *e, char *error)
{
	t_line	*line;
	int		len;

	line = e->file->begin;
	++e->file->error;
	len = ft_strlen(error);
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, line->y, error + len - line->s + 1);
	ft_dprintf(2, "missing terminating '\"' character\n");
	err_pointer(e->tty2, line->s, error + len);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}
