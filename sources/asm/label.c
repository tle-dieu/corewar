/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/07 05:07:41 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "asm.h"

static int	search_label(t_env *e, char *s, int len)
{
	t_label			*label;
	t_call			*call;

	label = e->file->label;
	while (label && (ft_strncmp(s, label->name, len) || label->name[len]))
		label = label->next;
	if (label)
	{
		if (label->index != -1) // enlever increment error
			redefine_label(e, s, label->y);
		else
		{
			label->index = e->file->i;
			call = label->call;
			if (!e->file->error)
				write_label_call(e, call);
		}
		return (1);
	}
	return (0);
}

static void	get_label(t_env *e, char *s, int len)
{
	t_label	*new;

	new = NULL;
	if (!(new = (t_label *)malloc(sizeof(t_label))))
		alloc_error(e);
	if (!(new->name = ft_strndup(s, len)))
	{
		free(new);
		alloc_error(e);
	}
	new->y = e->file->last->y;
	new->index = e->file->i;
	new->call = NULL;
	new->next = e->file->label;
	e->file->label = new;
}

int     	only_label(t_env *e, char **line)
{
	char	*s;

	s = *line + ft_strspn(*line, LABEL_CHARS);
	if (*s != LABEL_CHAR)
		return (0);
	e->file->champ_part = 1;
	if (!search_label(e, *line, s - *line))
		get_label(e, *line, s - *line);
	*line = s + ft_strspn(s + 1, SPACES) + 1;
	return (!**line);
}
