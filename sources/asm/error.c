/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/07 05:48:54 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <errno.h>
#include <stdlib.h>

int		alloc_error(t_env *e)
{
	ft_dprintf(2, line_error(ERR_FATAL, e->tty2));
	ft_dprintf(2, "%s\n", strerror(errno));
	free_lst_file(e);
	exit(EXIT_FAILURE);
	return (-1);
}

int		basic_error(t_env *e, char *str, char *err_string, int wave)
{
	++e->file->error;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, e->file->last->y, str - e->file->last->s + 1);
	ft_dprintf(2, err_string);
	err_pointer(e->tty2, e->file->last->s, str, 0);
	if (wave)
		err_wave(e->tty2, str, wave);
	ft_dprintf(2, "\n");
	return (1);
}

void	missing_cmd(t_env *e, unsigned char *header, int cmd)
{
	char	*scmd;
	char	*default_str;
	int		len;

	++e->file->warning;
	if (cmd == NAME_CMD)
		len = sizeof(DEFAULT_NAME);
	else
		len = sizeof(DEFAULT_COMMENT);
	default_str = (cmd == NAME_CMD ? DEFAULT_NAME : DEFAULT_COMMENT);
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	ft_dprintf(2, line_error(WARNING_FILE, e->tty2), e->file->name);
	ft_dprintf(2, "'%s' is undefined (set to '%s')\n", scmd, default_str);
	if (e->tty2)
		ft_dprintf(2, "{R}");
	ft_memcpy(header, default_str, len);
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

void	undefined_label(t_env *e, t_call *call, int *note, int tt)
{
	int len;

	++e->file->error;
	len = ft_strspn(call->s, LABEL_CHARS);
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, call->line->y, call->s - call->line->s);
	ft_dprintf(2, "label '%.*s' is undefined ", len, call->s);
	if (tt)
		ft_dprintf(2, "(%d other%s", tt, tt > 1 ? "s)" : ")");
	ft_dprintf(2, "\n");
	err_pointer(e->tty2, call->line->s, call->s, 0);
	ft_dprintf(2, "\n");
	if (!*note)
	{
		ft_dprintf(2, line_error(NOTE_LINE, e->tty2),
			e->file->name, call->line->y, call->s - call->line->s);
		ft_dprintf(2, "undefined label reported only once\n");
		*note = 1;
	}
	if (e->tty2)
		ft_dprintf(2, "{R}");
}

int		expect_str(t_env *e, char *error, int cmd)
{
	t_line	*line;
	char	*scmd;

	++e->file->error;
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	line = e->file->begin;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "expected string after %s\n", scmd);
	err_pointer(e->tty2, line->s, error + 1 - (!ft_strchr(SPACES, *error)), 0);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

void	redefine_label(t_env *e, char *error, int y)
{
	t_line *line;

	line = e->file->last;
	++e->file->warning;
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "redefinition of label '%.*s' not allowed",
		ft_strchr(error, LABEL_CHAR) - error, error);
	ft_dprintf(2, "previous definition line %d)\n", y);
	err_pointer(e->tty2, line->s, error, 0);
	ft_dprintf(2, "\n");
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
	err_pointer(e->tty2, line->s, error++, 0);
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
	err_pointer(e->tty2, e->file->last->s, error, 0);
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

	line = e->file->begin;
	++e->file->error;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "missing terminating '\"' character\n");
	err_pointer(e->tty2, line->s, error + ft_strlen(error), 0);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

void	cmd_part_champ(t_env *e, int cmd)
{
	t_line	*line;
	char	*scmd;
	char	*error;
	int		len;

	line = e->file->begin;
	++e->file->warning;
	error = line->s + ft_strspn(line->s, SPACES);
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "%s should be before instruction or label\n", scmd);
	err_pointer(e->tty2, line->s, error++, 0);
	len = ft_strclen(error, '"');
	len = error[len] == '"' ? len + ft_strclen(error + len + 1, '"') + 1
		: ft_strcspn(error, SPACES"\"");
	err_wave(e->tty2, error, len + (error[len] == '"'));
	ft_dprintf(2, "\n");
}

void	cmd_multiple_define(t_env *e, int cmd)
{
	t_line	*line;
	char	*scmd;
	char	*error;
	int		len;

	++e->file->warning;
	e->file->complete |= ALREADY_DEFINE;
	line = e->file->begin;
	error = line->s + ft_strspn(line->s, SPACES);
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "%s already defined (ignored)\n", scmd);
	err_pointer(e->tty2, line->s, error++, 0);
	len = ft_strclen(error, '"');
	len = error[len] == '"' ? len + ft_strclen(error + len + 1, '"') + 1
		: ft_strcspn(error, SPACES"\"");
	err_wave(e->tty2, error, len + (error[len] == '"'));
	ft_dprintf(2, "\n");
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
	err_pointer(e->tty2, e->file->begin->s, error, 0);
	err_wave(e->tty2, error, ft_strcspn(error + 1, SPACES"\""));
	ft_dprintf(2, "\n");
}
