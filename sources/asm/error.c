/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/03 14:25:03 by tle-dieu         ###   ########.fr       */
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
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name,
		e->file->last->y , str - e->file->last->s);
	ft_dprintf(2, err_string);
	err_pointer(e->tty2, e->file->last->s, str, 0);
	if (wave)
		err_wave(e->tty2, str, wave);
	ft_dprintf(2, "\n");
}

int		expect_str(t_env *e, char *error, int cmd) // 3
{
	t_line	*line;

	++e->file->error;
	line = e->file->begin;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, 
		line->y, error - line->s + 1);
	ft_dprintf(2, "expected string after %s\n",
		cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING);
	err_pointer(e->tty2, line->s, error + (ft_strchr(SPACES, *error) != NULL), 0);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		cmd_too_long(t_env *e, char *error, int cmd) // 2
{
	t_line	*line;
	char	*scmd;
	int		max;

	++e->file->error;
	max = cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH;
	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	line = e->file->begin;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "%s declaration too long (Max length: %d)\n", scmd, max);
	err_pointer(e->tty2, line->s, error++, 0);
	err_wave(e->tty2, error, ft_strclen(error, '"') + 1);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		unexpected_expression(t_env *e, char *error, int cmd) // 1
{
	char	*scmd;
	t_line	*line;

	++e->file->error;
	line = e->file->last;
	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "unexpected expression in %s declaration\n", scmd);
	err_pointer(e->tty2, e->file->last->s, error, 0);
	if (ft_strcspn(e->file->last->s, "\"") + e->file->last->s > error)
		err_wave(e->tty2, error, ft_strcspn(error, SPACES"\"") - 1);
	else
		err_wave(e->tty2, error, ft_strcspn(error, SPACES) - 1);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		missing_quote(t_env *e, char *error) // 4
{
	t_line	*line;

	line = e->file->begin;
	++e->file->error;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "missing terminating '\"' character\n");
	err_pointer(e->tty2, line->s, error + ft_strlen(error), 0);
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		cmd_multiple_define(t_env *e,int cmd) // 5
{
	t_line	*line;
	char	*scmd;
	char	*error;
	int len;

	++e->file->warning;
	e->file->complete |= ALREADY_DEFINE;
	line = e->file->begin;
	error = line->s + ft_strspn(line->s, SPACES);
	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2), e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "%s already defined (ignored)\n", scmd); // passer en warning
	err_pointer(e->tty2, line->s, error++, 0);
	len = ft_strclen(error, '"');
	len = error[len] == '"' ? len + ft_strclen(error + len + 1, '"') + 1
	: ft_strcspn(error, SPACES"\"");
	err_wave(e->tty2, error, len + (error[len] == '"'));
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}

int		invalid_cmd(t_env *e, char *error, int cmd) // 6
{
	t_line	*line;
	char	*scmd;

	++e->file->warning;
	line = e->file->begin;
	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2), e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "invalid command '%.*s'{R}\n", ft_strcspn(error, SPACES"\""), error);
	err_pointer(e->tty2, e->file->begin->s, error, 0);
	err_wave(e->tty2, error, ft_strcspn(error + 1, SPACES"\""));
	ft_dprintf(2, "\n");
	return (-(e->file->error >= 20));
}
