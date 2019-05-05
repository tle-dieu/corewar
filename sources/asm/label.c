/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:24:04 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/05 16:38:59 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"
#include <stdlib.h>

int		create_call(t_env *e, t_inst *inst, char *s, t_label *label, int i)
{
	t_label *new;
	t_call	*call;

	new = NULL;
	if (!label)
	{
		if (!(new = (t_label *)malloc(sizeof(t_label))))
			exit (0); // alloc error
		label = new;
		new->index = -1;
		label->call = NULL;
		new->next = e->file->label;
		e->file->label = new;
		if (!(new->name = ft_strndup(s, e->i)))
		{
			free(new);
			exit(0); // alloc error
		}
	}
	if (!(call = (t_call *)malloc(sizeof(t_call))))
	{
		if (new)
			free(new->name); // remplacer par fonction free call ?
		free(new);
		exit(0);
	}
	e->file->last->free = -(e->file->last->free == 0);
	call->line = e->file->last;
	call->index_inst = e->file->i;
	call->index_call = inst->index;
	call->s = s;
	call->size = inst->s[i]; //envoyer que inst et pas i
	call->next = label->call;
	label->call = call;
	return (1);
}

void	get_label_call(t_env *e, t_inst *inst, char *s, int i)
{
	int		len;
	t_label *label;

	label = e->file->label;
	len = ft_strcspn(s, SPACES",");
	e->i = len;
	while (label)
	{
		if (!ft_strncmp(s, label->name, len) && !label->name[len])
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
		create_call(e, inst, s, label, i); // separer en deux pour trop args
}

int		label_is_good(t_env *e, char *str)
{
	char	*tmp;
	int		err;

	err = 0;
	tmp = str;
	tmp += ft_strspn(tmp, LABEL_CHARS);
	if (*tmp != *SEPARATOR_CHAR && *tmp && !ft_strchr(SPACES, *tmp))
	{
		basic_error(e, tmp, "illegal character for label\n", 0);
		err = 1;
	}
	tmp += ft_strcspn(tmp, SPACES",");
	tmp += ft_strspn(tmp, SPACES);
	if (*tmp && *tmp != *SEPARATOR_CHAR)
	{
		basic_error(e, tmp, "unexpected expression after parameter\n",
			ft_strcspn(tmp, SEPARATOR_CHAR) - 1);
		err = 1;
	}
	return (!err);
}
