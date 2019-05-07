/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 06:01:24 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/07 06:04:02 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

static char	*pass_line(char *s)
{
	int		i;

	s += ft_strspn(s, SPACES);
	if (!*s || ft_strchr(COMMENT_CHAR, *s))
		return (NULL);
	if (*(s + (i = ft_strcspn(s, COMMENT_CHAR)))
			&& (!(ft_ncount_occ(s, '"', i) & 1) || !ft_strchr(s + i, '"')))
		*(s + i) = '\0';
	return (s);
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
		if (!e->file->begin && !(without_space = pass_line(*line)))
		{
			e->file->line_nb++;
			free(*line);
			*line = NULL;
		}
		else
			break ;
	}
	if (ret <= 0)
		return (ret == -1 ? alloc_error(e) : 0);
	assign_line(e, *line);
	*line = without_space ? without_space : *line;
	return (1);
}
