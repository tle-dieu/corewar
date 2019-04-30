/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:43:32 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/30 09:23:55 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>
#include <unistd.h>

int		parse_cmd(t_env *e, char *s, unsigned char *cp, int cmd)
{
	char	buff[BS_HEADER + 1];
	int		i;
	char	*t;
	int		end;

	i = 0;
	end = 0;
	if (e->file->complete & (cmd + 1)) // gerer error comment trouve et instructions trouvees
		if (error_header(e, 5, e->file->begin->s, cmd) == -1)
			return (0);
	e->file->complete |= cmd + 1;
	if (!(t = ft_strchr(s, '"')))
	{
		error_header(e, 3, s, cmd);
		return (check_end_str(e, s, cmd, 0));
	}
	if (check_end_str(e, s, cmd, '"') == -1)
		return (0);
	s = t + 1;
	while (!end)
	{
		while (*s && *s != '"')
		{
			if (i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
			{
				i = -1;
				error_header(e, 2, ft_strchr(e->file->begin->s, '"'), cmd);
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
			return (error_header(e, 4, e->file->begin->s + ft_strlen(e->file->begin->s), cmd));
		else if (i != -1)
			buff[i++] = '\n';
	}
	if (!e->file->error)
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
		cmd = 0;
	}
	else if (!ft_strncmp(COMMENT_CMD_STRING, line, sizeof(COMMENT_CMD_STRING) - 1))
	{
		line += sizeof(COMMENT_CMD_STRING) - 1; 
		cmd = 1;
	}
	if (cmd == -1 || !ft_strchr(SPACES"\"", *line))
		error_header(e, 6, cmd != -1 ? tmp : line, -1);
	else
		!cmd ? parse_cmd(e, line, cp, cmd)
			: parse_cmd(e, line, cp + PROG_NAME_LENGTH + 8, cmd);
}

void	get_bytecode(t_env *e, unsigned char *cp)
{
	char	*line;

	while (e->file->error < 20 && add_line(e, &line) == 1)
	{
		ft_printf("get line: '%s'\n", line);
		if (*line == '.')
		{
			ft_printf("command\n");
			get_cmd(e, cp + 4, line);
		}
		else if (*line)
		{
			ft_printf("only label ?\n");
			if (!only_label(e, &line, cp + HEADER_SIZE))
			{
				ft_printf("inst\n");
				parse_inst(e, line, cp + HEADER_SIZE);
			}
		}
		ft_printf("free line get bytecode\n");
		free_line(&e->file->begin);
		if (!e->file->uniq_line)
			free_line(&e->file->last);
	}
	if (e->file->error >= MAX_ERROR)
	{
		ft_dprintf(2, line_error(ERR_FATAL, e->tty2));
		ft_dprintf(2, "too many errors emitted, stopping now\n");
	}
	print_call_error(e);
}
