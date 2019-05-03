/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/03 17:43:01 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "asm.h"
#include "op.h"

//au moment du free de call, segfault si deux label sur mm ligne
//verifier tous les remplissages de buffs

#include <unistd.h>

void	print_buff(t_buff *buff)
{
	int j;
	int i;
	int k;

	k = 0;
	ft_printf("print buff\n");
	while (buff)
	{
		i = 0;
		j = 0;
		ft_printf("BUFF: %d\n", k);
		while (i < buff->len)
		{
			ft_printf("%02x ", buff->s[i++]);
			if (++j == 16)
			{
				ft_printf("\n");
				j = 0;
			}
		}
		ft_printf("\n");
		++k;
		buff = buff->next;
	}
}

//verifier si il est possible d'avoir un buff non cree mais normalement nn
void	write_label_call(t_env *e, t_call *call)
{
	int				i;
	int				byte;
	t_buff			*buff;

	while (call)
	{
		buff = e->file->begin_buff;
		if (PRINT)
		{
			ft_printf("{cyan}label call:{R}\n");
			ft_printf(" -index: %zu\n", call->index_call);
		}
		while (call->index_call >= BS_ASM)
		{
			call->index_call -= buff->len;
			if (PRINT)
				ft_printf(" -index: %zu\n", call->index_call);
			buff = buff->next;
		}
		i = 0;
		while (call->size--)
		{
			if (call->index_call >= BS_ASM)
			{
				if (PRINT)
				{
					ft_printf("{yellow} -new index file: %d\n", call->index_call);
					ft_printf(" -new avance{R}\n");
				}
				call->index_call = 0;
				buff = buff->next;
			}
			if (PRINT)
				ft_printf("index file: %d\n", call->index_call);
			byte = (e->file->i - call->index_inst) >> call->size * 8;
			buff->s[call->index_call++] = byte;
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
void	get_label(t_env *e, char *s, int len)
{
	t_label	*new;

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
	char	*s;

	s = *line + ft_strspn(*line, LABEL_CHARS);
	if (*s != LABEL_CHAR)
		return (0);
	if (!search_label(e, *line, s - *line))
		get_label(e, *line, s - *line);
	*line = s + ft_strspn(s + 1, SPACES) + 1;
	return (!**line);
}
