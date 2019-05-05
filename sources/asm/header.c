/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:43:32 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/05 14:43:23 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>
#include <unistd.h>

int		parse_cmd(t_env *e, char *s, unsigned char *cp, int cmd)
{
	char	buff[BS_HEADER];
	int		i;
	char	*t;
	int		end;

	i = 0;
	end = 0;
	if (e->file->champ_part)
		cmd_part_champ(e, cmd);
	if (e->file->complete & cmd) // gerer error comment trouve et instructions trouvees
		cmd_multiple_define(e, cmd);
	e->file->complete |= cmd;
	if (!(t = ft_strchr(s, '"')))
		return (expect_str(e, s, cmd) == -1 || check_end_str(e, s, cmd, 0));
	if (check_end_str(e, s, cmd, '"') == -1)
		return (0);
	s = t + 1;
	while (!end)
	{
		while (*s && *s != '"')
		{
			if (i >= (cmd == NAME_CMD ? PROG_NAME_LENGTH : COMMENT_LENGTH))
			{
				i = -1;
				if (cmd_too_long(e, ft_strchr(e->file->begin->s, '"'), cmd) == -1)
					return (0);
			}
			else if (i != -1)
				buff[i++] = *s;
			s++;
		}
		if (*s == '"' && (end = 1))
		{
			if (check_end_str(e, s + 1, cmd, 0) <= 0)
				return (0);
		}
		else if (add_line(e, &s) != 1)
			return (missing_quote(e, e->file->begin->s));
		else if (i >= (cmd == NAME_CMD ? PROG_NAME_LENGTH : COMMENT_LENGTH))
		{
			i = -1;
			if (cmd_too_long(e, ft_strchr(e->file->begin->s, '"'), cmd) == -1) // remplacer par fonction
				return (0);
		}
		else if (i != -1)
			buff[i++] = '\n';
	}
	if (!e->file->error && !(e->file->complete & ALREADY_DEFINE))
		while (i--)
			*(cp + i) = buff[i];
	return (1);
}

//verifier si line = NULL est utile

void	get_cmd(t_env *e, unsigned char *cp, char *line)
{
	int cmd;
	char *tmp;

	cmd = -1;
	tmp = line;
	if (!ft_strncmp(line, NAME_CMD_STRING, sizeof(NAME_CMD_STRING) - 1))
	{
		line += sizeof(NAME_CMD_STRING) - 1; 
		cmd = NAME_CMD;
	}
	else if (!ft_strncmp(COMMENT_CMD_STRING, line, sizeof(COMMENT_CMD_STRING) - 1))
	{
		line += sizeof(COMMENT_CMD_STRING) - 1; 
		cmd = COMMENT_CMD;
	}
	if (cmd == -1 || !ft_strchr(SPACES"\"", *line))
		invalid_cmd(e, cmd != -1 ? tmp : line, -1);
	else
		cmd == NAME_CMD ? parse_cmd(e, line, cp, cmd)
			: parse_cmd(e, line, cp + PROG_NAME_LENGTH + 8, cmd);
}

void	get_bytecode(t_env *e, unsigned char *header)
{
	char	*line;

	while (e->file->error < 20 && add_line(e, &line) == 1)
	{
		if (*line == '.')
			get_cmd(e, header + 4, line);
		else if (*line)
		{
			if (!only_label(e, &line))
				parse_inst(e, line);
		}
		if (PRINT)
		{
			ft_printf("{yellow}in get_bytecode :: {R}\n");
			ft_printf("last: %p != begin: %p\n", e->file->last, e->file->begin);
		}
		if (e->file->last != e->file->begin)
		{
			if (PRINT)
				ft_printf("{cyan}before e->file->last {R}%p\n", e->file->last);
			free_line(&e->file->last);
			if (PRINT)
				ft_printf("{bold}{red}after e->file->last {R}%p\n", e->file->last);
		}
		else
			e->file->last = NULL;
		if (PRINT)
			ft_printf("{cyan}before e->file->begin {R}%p\n", e->file->begin);
		free_line(&e->file->begin);
		if (PRINT)
			ft_printf("{red}{bold}after e->file->begin {R}%p\n", e->file->begin);
	}
	check_label_call(e);
	if (e->file->error >= MAX_ERROR)
	{
		ft_dprintf(2, line_error(ERR_FATAL, e->tty2));
		ft_dprintf(2, "too many errors emitted, stopping now\n");
	}
}
