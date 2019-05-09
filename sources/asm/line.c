/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 06:01:24 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 02:51:39 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>

static char	*pass_line(char *s, int quote)
{
	char	*tmp;
	int		len;
	int		i;

	len = sizeof(COMMENT_CHAR) - 1;
	if (!quote)
	{
		s += ft_strspn(s, SPACES);
		if (!*s || ft_strchr(COMMENT_CHAR, *s))
			return (NULL);
	}
	tmp = s;
	while (*s)
	{
		i = len;
		if (*s == '"')
			quote = (quote & 1) + 1;
		else
			while (i-- && *s)
				if (*s == COMMENT_CHAR[i] && !(quote & 1))
					*s-- = '\0';
		s++;
			
	}
	return (tmp);
}

static void	assign_line(t_env *e, char *line)
{
	t_line *new;

	if (!(new = (t_line *)malloc(sizeof(t_line))))
	{
		free(line);
		alloc_error(e);
	}
	new->free = 1;
	new->s = line;
	new->y = e->file->line_nb++;
	if (!e->file->begin)
		e->file->begin = new;
	else
	{
		if (e->file->last != e->file->begin)
			free_line(&e->file->last, 0);
		e->file->last = new;
	}
	e->file->last = new;
}

int			add_line(t_env *e, char **line)
{
	int		ret;
	char	*without_space;

	without_space = NULL;
	*line = NULL;
	while ((ret = get_next_line(e->file->fd, line)) > 0)
	{
		if (!(without_space = pass_line(*line, e->file->begin != NULL)))
		{
			e->file->line_nb++;
			free(*line);
			*line = NULL;
		}
		else
			break ;
	}
	if (ret == -1)
		alloc_error(e);
	if (!ret)
		return (0);
	assign_line(e, *line);
	*line = without_space ? without_space : *line;
	return (1);
}
