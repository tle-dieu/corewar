/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/26 18:13:54 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "asm.h"
#include "op.h"

//au moment du free de call, segfault si deux label sur mm ligne

int		search_label(t_env *e, char *s, int len, unsigned char *cp)
{
	t_label *label;
	t_call	*call;

	label = e->actual->label;
	while (label && (ft_strncmp(s, label->name, len) || label->name[len]))
		label = label->next;
	if (label)
	{
		if (label->index != -1)
			ft_printf("{#ff3333}error double assign label{R}\n");
		else
		{
			label->index = e->actual->i;
			call = label->call;
			while (call)
			{
				(void)cp;
				/* while (call->size-- > 0) */
					/* *(cp++ + call->index_call) = (e->actual->i - call->index_inst) >> call->size; */
				call = call->next;
			}
		}
		return (1);
	}
	return (0);
}
//checker pour labels commencant pareil
void	get_label(t_env *e, char *s)
{
	t_label	*new;
	int		len;

	if (*(s + (len = ft_strspn(s, LABEL_CHARS))) != LABEL_CHAR)
		ft_printf("{#ff3333}label char interdit: {R}%c\n", *(s + len));
	new = NULL;
	if (!(new = (t_label *)malloc(sizeof(t_label))))
		alloc_error(e);
	if (!(new->name = ft_strndup(s, len)))
	{
		free(new);
		alloc_error(e);
	}
	ft_printf(NAME_C"new label: {R}%s\n", new->name);
	new->index = e->actual->i;
	new->call = NULL;
	new->next = e->actual->label;
	e->actual->label = new;
}

int     only_label(t_env *e, char **line, unsigned char *cp)
{
	int		i;
	int		len;
	char	*s;

	s = *line;
	len = sizeof(SPACES) - 1;
	while (*s != LABEL_CHAR)
	{
		i = len;
		if (!*s)
			return (0);
		while (i--)
			if (SPACES[i] == *s)
				return (0);
		s++;
	}
	ft_printf(NAME_C"LABEL{R}\n");
	if (!search_label(e, *line, s - *line, cp))
		get_label(e, *line);
	*line = s + ft_strspn(s + 1, SPACES) + 1;
	return (!**line);
}

int		get_champ(t_env *e, char *s, unsigned char *cp)
{
	print_label(e);
	if (!only_label(e, &s, cp))
		parse_inst(e, s, cp);
	(void)cp;
	return (1);
}
