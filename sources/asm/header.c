/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:43:32 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/13 15:30:19 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>
#include <unistd.h>

int		multi_line(t_file *file, char *buff, int i, int cmd)
{
	char *line;
	char *s;
	int	end;

	end = -1;
	while (end == -1)
	{
		if (add_line(&line, file) != 1)
			return (0);
		buff[i++] = '\n';
		s = line;
		while (*s && *s != '"')
		{
			if (i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
				return (error_header(file, 2, ft_strchr(file->begin->s, '"'), cmd));
			buff[i++] = *s++;
		}
		if (*s == '"')
			end = !check_end_str(&s);
	}
	buff[i] = '\0';
	return (error_header(file, !end, s, cmd));
}

int		get_name(t_file *file, char *s, unsigned char *cp, int cmd)
{
	char	buff[PROG_NAME_LENGTH + 1];
	int		i;
	char	*t;

	i = 0;
	if (!(t = ft_strchr(s, '"')))
		return (error_header(file, 3, s, cmd));
	s = t;
	while (*++s && *s != '"')
	{
		if (i >= (cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH))
			return (error_header(file, 2, ft_strchr(file->begin->s, '"'), cmd));
		buff[i++] = *s;
	}
	if (!*s)
	{
		if (!(multi_line(file, buff, i, cmd)))
			return (0);
	}
	else
		buff[i] = '\0';
	if (check_end_str(&s))
		return (error_header(file, 1, s, cmd));
	i = 0;
	while (buff[i])
		*cp++ = buff[i++];
	return (1);
}

//verifier si line = NULL est utile
int		get_header(t_file *file, unsigned char *cp)
{
	char	*line;
	int		i;

	line = NULL;
	while (add_line(&line, file) == 1)
	{
		i = 0;
		while (line[i])
		{
			if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			{
				if (!ft_strncmp(line + i, NAME_CMD_STRING, sizeof(NAME_CMD_STRING) - 1))
				{
					if (!(get_name(file, line + i + sizeof(NAME_CMD_STRING), cp, 0)))
						exit(0);
					break ;
				}
				else if (!ft_strncmp(COMMENT_CMD_STRING, line + i, sizeof(COMMENT_CMD_STRING) - 1))
				{
					if (!(get_name(file, line + i + sizeof(COMMENT_CMD_STRING), cp + PROG_NAME_LENGTH + 8, 1)))
						exit(0);
					break ;
				}
			}
			i++;
		}
		line = NULL;
	}
	return (1);
}
