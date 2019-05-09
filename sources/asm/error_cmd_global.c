/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_cmd_global.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 00:27:13 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 17:44:07 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

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
	err_pointer(e->tty2, line->s, error++);
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
	err_pointer(e->tty2, line->s, error++);
	len = ft_strclen(error, '"');
	len = error[len] == '"' ? len + ft_strclen(error + len + 1, '"') + 1
		: ft_strcspn(error, SPACES"\"");
	err_wave(e->tty2, error, len + (error[len] == '"'));
	ft_dprintf(2, "\n");
}
