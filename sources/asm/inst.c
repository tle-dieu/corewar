/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/26 15:06:07 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "asm.h"
#include "op.h"

//au moment du free de call, segfault si deux label sur mm ligne

int		search_label(t_env *e, char *s, int len)
{
	t_label *label;
	t_call	*call;

	ft_printf("search label\n");
	label = e->actual->label;
	while (label && (ft_strncmp(s, label->name, len) || label->name[len]))
		label = label->next;
	if (label)
	{
		if (label->index != -1)
			ft_printf("{#ff3333}error double assign label{R}\n");
		else
		{
			ft_printf("{yellow}label call find\n{R}");
			label->index = e->actual->i;
			call = label->call;
			while (call)
			{
				ft_printf(MAGIC_C"line: '%s' call: '%s' index: %02x\n{R}", e->actual->last->s, call->line->s, e->actual->i - call->index);
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

	ft_printf("get label\n");
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

int     only_label(t_env *e, char **line)
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
	if (!search_label(e, *line, s - *line))
		get_label(e, *line);
	*line = s + ft_strspn(s + 1, SPACES) + 1;
	ft_printf("line after label: %s\n", *line);
	return (!**line);
}

int		get_champ(t_env *e, char *s, unsigned char *cp)
{
	print_label(e);
	if (!only_label(e, &s))
	{
		ft_printf(NAME_C"INSTRUCTION{R}\n");
		parse_inst(e, s);
	}
	(void)cp;
	return (1);
}
