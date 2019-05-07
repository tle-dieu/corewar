/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label_call.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:24:04 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/07 05:39:33 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void    check_label_call(t_env *e)
{
	t_label *label;
	t_call  *call;
	int		note;
	int		tt;

	label = e->file->label;
	note = 0;
	while (label && e->file->error < MAX_ERROR)
	{
		if (label->index == -1)
		{
			tt = 0;
			call = label->call;
			while (call->next)
			{
				++tt;
				undefined_label(e, call, &note, tt);
				call = call->next;
			}
		}
		label = label->next;
	}
}

void			write_label_call(t_env *e, t_call *call)
{
	int				i;
	int				byte;
	t_buff			*buff;

	while (call)
	{
		buff = e->file->begin_buff;
		while (call->index_call >= BS_ASM)
		{
			call->index_call -= buff->len;
			buff = buff->next;
		}
		i = 0;
		while (call->size--)
		{
			if (call->index_call >= BS_ASM)
			{
				call->index_call = 0;
				buff = buff->next;
			}
			byte = (e->file->i - call->index_inst) >> call->size * 8;
			buff->s[call->index_call++] = byte;
		}
		call = call->next;
	}
}

static t_label	*add_undefined_label(t_env *e, char *s)
{
	t_label *new;

	if (!(new = (t_label *)malloc(sizeof(t_label))))
		alloc_error(e);
	new->index = -1;
	new->call = NULL;
	new->next = e->file->label;
	e->file->label = new;
	new->name = NULL;
	if (!(new->name = ft_strndup(s, e->i))) // erreur partie sans raison (a retester)
		alloc_error(e);
	return (new);
}

static int		create_call(t_env *e, t_inst *inst, char *s, t_label *label)
{
	t_call	*call;

	call = NULL;
	if (!(call = (t_call *)malloc(sizeof(t_call))))
		alloc_error(e);
	e->file->last->free = -(e->file->last->free == 0);
	call->line = e->file->last;
	call->index_inst = e->file->i;
	call->index_call = inst->index;
	call->s = s;
	call->size = inst->s[e->i];
	call->next = label->call;
	label->call = call;
	return (1);
}

void	get_label_call(t_env *e, t_inst *inst, char *s, int i)
{
	t_label *label;

	label = e->file->label;
	e->i = ft_strcspn(s, END_PARAM);
	while (label)
	{
		if (!ft_strncmp(s, label->name, e->i) && !label->name[e->i])
		{
			if (label->index != -1)
			{
				inst->p[i] = label->index - e->file->i;
				return ;
			}
			break ;
		}
		label = label->next;
	}
	if (!label || label->index == -1)
	{
		if (!label)
			label = add_undefined_label(e, s);
		e->i = i;
		create_call(e, inst, s, label);
	}
}
