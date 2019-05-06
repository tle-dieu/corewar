/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/06 02:36:01 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"
#include <stdlib.h>

int		get_curr_inst(char *str)
{
	int	i;

	i = 0;
	while (g_op_tab[i].label
		&& (ft_strncmp(str, g_op_tab[i].label, g_op_tab[i].len)
		|| ft_isalnum(str[g_op_tab[i].len])))
		i++;
	if (!g_op_tab[i].label)
		return (42);
	return (i + 1);
}

void get_ocp(t_inst *inst)
{
	int			i;
	int			ocp;
	int			tab[3];

	tab[0] = 64;
	tab[1] = 16;
	tab[2] = 4;
	i = 0;
	ocp = 0;
	while (i < inst->nb_p)
	{
		ocp += tab[i] * inst->t[i];
		i++;
	}
	inst->ocp = ocp;
}


void	add_buff(t_env *e)
{
	t_buff	*new;

	if (!(new = (t_buff *)malloc(sizeof(t_buff))))
		alloc_error(e);
	new->next = NULL;
	if (!e->file->begin_buff)
		e->file->begin_buff = new;
	else
		e->file->buff->next = new;
	new->len = 0;
	e->file->buff = new;
}

void	write_inst(t_env *e, t_inst *inst)
{
	int i;
	int	k;
	int j;
	int	buff[MAX_LEN_INST];

	i = 0;
	j = 0;
	if (!inst->error && !e->file->error)
	{
		buff[j++] = inst->op;
		if (g_op_tab[inst->op - 1].ocp)
			buff[j++] = inst->ocp;
		while (i < inst->nb_p)
		{
			k = inst->s[i];
			while (k--)
				buff[j++] = inst->p[i] >> k * 8;
			i++;
		}
	}
	if (!e->file->begin_buff && j)
		add_buff(e);
	if (e->file->buff)
		k = e->file->buff->len;
	i = 0;
	while (i < j)
	{
		if (e->file->buff->len >= BS_ASM)
			add_buff(e);
		e->file->buff->s[e->file->buff->len++] = buff[i++];
	}
	if (!inst->error)
		e->file->i = inst->index;
}

t_inst	*parse_inst(t_env *e, char *str)
{	
	t_inst	inst;
	char	*tmp;

	inst = (t_inst){.ocp = 0}; // verifier norme
	if ((inst.op = get_curr_inst(str)) <= 16)
	{
		inst.nb_p = g_op_tab[inst.op - 1].nb_param;
		tmp = str + ft_strlen(g_op_tab[inst.op - 1].label);
		tmp += ft_strspn(tmp, SPACES);
		check_params(e, tmp, &inst);
		if (g_op_tab[inst.op - 1].ocp)
			get_ocp(&inst);
		if (!e->file->champ_part)
			e->file->champ_part = 1;
	}
	else
		error_unknow_inst(e, str);
	write_inst(e, &inst);
	return (NULL);
}
