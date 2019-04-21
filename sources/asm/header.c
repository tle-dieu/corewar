/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:43:32 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/21 07:55:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>
#include <unistd.h>

int		too_long(t_env *e, char *s, int cmd)
{
	char *line;
	int	i;

	i = 0;
	line = NULL;
	error_header(e, 2, ft_strchr(e->actual->begin->s, '"'), cmd);
	while (!ft_strchr(!i++ ? e->actual->last->s : s, '"'))
		if (add_line(e, &line) != 1) // mettre limite max nb lines dans add line pour erreur de malloc
			return (error_header(e, 4, e->actual->begin->s + ft_strlen(e->actual->begin->s), cmd) - 1);
	s = ft_strchr(i == 1 ? s : e->actual->last->s, '"');
	return (error_header(e, check_end_str(&s) != NULL, s, cmd));
}

int		multi_line(t_env *e, char *buff, int *i, int cmd)
{
	char *line;
	char *s;
	int	end;

	end = -1;
	while (end == -1)
	{
		if (add_line(e, &line) != 1)
			return (-1);
		if (line)
		{
			buff[(*i)++] = '\n';
			s = line;
			while (*s && *s != '"')
			{
				if (*i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
					return (too_long(e, line, cmd));
				buff[(*i)++] = *s++;
			}
			if (*s == '"')
				end = !check_end_str(&s);
		}
		else
			free(line);
	}
	return (error_header(e, !end, s, cmd)); // retirer return error
}

int		parse_cmd(t_env *e, char *s, unsigned char *cp, int cmd)
{
	char	buff[BS_HEADER + 1];
	int		i;
	char	*t;

	i = 0;
	ft_printf("get %s\n", cmd ? COMMENT_CMD_STRING :  NAME_CMD_STRING);
	if (e->actual->complete & (cmd + 1)) // gerer error comment trouve et instructions trouvees
		error_header(e, 5, e->actual->begin->s, cmd);
	if (!(t = ft_strchr(s, '"')))
	{
		error_header(e, 3, s, cmd);
		return (error_header(e, check_end_str(&s) != NULL, s, cmd));
	}
	s = t;
	while (*++s && *s != '"')
	{
		if (i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
			return (too_long(e, s, cmd));
		buff[i++] = *s;
	}
	if (!*s)
	{
		if (multi_line(e, buff, &i, cmd) == -1)
			return (0);
	}
	else if (check_end_str(&s))
		return (error_header(e, 1, s, cmd));
	e->actual->complete |= cmd + 1;
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
	if (cmd == -1 || !ft_strchr(SPACES, *line))
		error_header(e, 6, cmd != -1 ? tmp : line, -1);
	else
		!cmd ? parse_cmd(e, line, cp, cmd)
			: parse_cmd(e, line, cp + PROG_NAME_LENGTH + 8, cmd);
}

void	get_header(t_env *e, unsigned char *cp)
{
	char	*line;
	int		i;

	while (e->actual->error < 20 && add_line(e, &line) == 1)
	{
		i = 0;
		if (line)
		{
			while (line[i])
			{
				if (!ft_strchr(SPACES"\n", line[i]))
				{
					if (line[i] == '.')
						get_cmd(e, cp, line + i);
					else if (line[i])
					{
						++e->actual->nb_inst;
						get_inst(e, line + i, cp + PROG_NAME_LENGTH + COMMENT_LENGTH + 12);
					}
					free_line(e->actual);
					break ;
				}
				i++;
			}
		}
	}
	//if error >= 20 print too many error
}
