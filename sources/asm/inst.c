/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/03 03:50:27 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "asm.h"
#include "op.h"

//au moment du free de call, segfault si deux label sur mm ligne
//verifier tous les remplissages de buffs

#include <unistd.h>

//verifier si il est possible d'avoir un buff non cree mais normalement nn
void	write_label_call(t_env *e, t_call *call)
{
	int				i;
	int				byte;
	t_buff			*buff;

	while (call)
	{
		buff = e->file->begin_buff;
		if (PRINT || 1)
		{
			ft_printf("{cyan}label call:{R}\n");
			ft_printf(" -index: %zu\n", call->index_call);
		}
		while (call->index_call >= BS_ASM)
		{
			call->index_call -= buff->len;
			if (PRINT || 1)
				ft_printf(" -index: %zu\n", call->index_call);
			buff = buff->next;
		}
		i = 0;
		while (call->size--)
		{
			byte = (e->file->i - call->index_inst) >> call->size * 8;
			if (i + call->index_call >= BS_ASM)
			{
				if (PRINT || 1)
				{
					ft_printf("{yellow} -new index file: %d index buff: %d\n", i + call->index_call, i);
					ft_printf(" -new avance{R}\n");
				}
				call->index_call = -i;
				buff = buff->next;
			}
			if (PRINT || 1)
				ft_printf("index file: %d index buff: %d\n", i + call->index_call, i);
			buff->s[i++ + call->index_call] = byte;
		}
		call = call->next;
	}
}

int		search_label(t_env *e, char *s, int len)
{
	t_label			*label;
	t_call			*call;

	label = e->file->label;
	while (label && (ft_strncmp(s, label->name, len) || label->name[len]))
		label = label->next;
	if (label)
	{
		if (label->index != -1 && ++e->file->error) // enlever increment error
			ft_dprintf(2, "{#ff3333}error double assign label{R}\n");
		else
		{
			label->index = e->file->i;
			call = label->call;
			if (PRINT)
				ft_printf("error: %d\n", e->file->error);
			if (!e->file->error)
				write_label_call(e, call);
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
		ft_dprintf(2, "{#ff3333}label char interdit: {R}%c\n", *(s + len));
	new = NULL;
	if (PRINT)
		ft_printf("{yellow}label OK: '%.*s'\n{R}", len + 1, s);
	if (!(new = (t_label *)malloc(sizeof(t_label))))
		alloc_error(e);
	if (!(new->name = ft_strndup(s, len)))
	{
		free(new);
		alloc_error(e);
	}
	new->index = e->file->i;
	new->call = NULL;
	new->next = e->file->label;
	e->file->label = new;
}

int     only_label(t_env *e, char **line)
{
	int		len;
	char	*s;
	int		i;

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
	if (!search_label(e, *line, s - *line))
		get_label(e, *line);
	*line = s + ft_strspn(s + 1, SPACES) + 1;
	return (!**line);
}
