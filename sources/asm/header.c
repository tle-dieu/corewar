/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:43:32 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/17 23:28:47 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>
#include <unistd.h>

int		multi_line(t_env *e, char *buff, int *i, int cmd)
{
	char *line;
	char *s;
	int	end;

	end = -1;
	while (end == -1)
	{
		if (add_line(e, &line) != 1)
			return (0);
		buff[(*i)++] = '\n';
		s = line;
		while (*s && *s != '"')
		{
			if (*i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
				return (error_header(e, 2, ft_strchr(e->actual->begin->s, '"'), cmd));
			buff[(*i)++] = *s++;
		}
		if (*s == '"')
			end = !check_end_str(&s);
	}
	return (error_header(e, !end, s, cmd)); // retirer return error
}

int		get_cmd(t_env *e, char *s, unsigned char *cp, int cmd)
{
	char	buff[BS_HEADER + 1];
	int		i;
	char	*t;

	i = 0;
	ft_printf("get %s\n", cmd ? COMMENT_CMD_STRING :  NAME_CMD_STRING);
	if (!(t = ft_strchr(s, '"')))
		return (error_header(e, 3, s, cmd));
	s = t;
	while (*++s && *s != '"')
	{
		if (i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
			return (error_header(e, 2, ft_strchr(e->actual->begin->s, '"'), cmd));
		buff[i++] = *s;
	}
	if (!*s)
	{
		if (!(multi_line(e, buff, &i, cmd)))
			return (0); // gerer pas de quote a la fin (missing terminating '"' character)
	}
	else if (check_end_str(&s))
		return (error_header(e, 1, s, cmd));
	while (i--)
		*(cp + i) = buff[i];
	return (1); //return plus necessaires toute la fonction
}

//verifier si line = NULL est utile
int		get_header(t_env *e, unsigned char *cp)
{
	char	*line;
	int		i;

	line = NULL;
	while (add_line(e, &line) == 1)
	{
		i = 0;
		while (line[i])
		{
			if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			{
				if (!ft_strncmp(line + i, NAME_CMD_STRING, sizeof(NAME_CMD_STRING) - 1))
					get_cmd(e, line + i + sizeof(NAME_CMD_STRING), cp, 0);
				else if (!ft_strncmp(COMMENT_CMD_STRING, line + i, sizeof(COMMENT_CMD_STRING) - 1))
					get_cmd(e, line + i + sizeof(COMMENT_CMD_STRING), cp + PROG_NAME_LENGTH + 8, 1);
				else if (line[i])
				{
					ft_printf("{#ff3333}line:{R} '%s' {#ff3333}is an instruction{R}\n", line);
					return (1);
				}
				free_line(e->actual);
				break ;
			}
			i++;
		}
		line = NULL;
	}
	return (1);
}
