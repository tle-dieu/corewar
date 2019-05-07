/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 23:48:19 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/07 23:48:26 by tle-dieu         ###   ########.fr       */
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

void	get_ocp(t_inst *inst)
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

	i = -1;
	if (!e->file->begin_buff && !inst->error && !e->file->error)
		add_buff(e);
	if (!inst->error && !e->file->error)
	{
		e->file->buff->s[e->file->buff->len++] = inst->op;
		if (e->file->buff->len >= BS_ASM)
			add_buff(e);
		if (g_op_tab[inst->op - 1].ocp)
			e->file->buff->s[e->file->buff->len++] = inst->ocp;
		if (e->file->buff->len >= BS_ASM)
			add_buff(e);
		while (++i < inst->nb_p)
			while (inst->s[i]--)
			{
				e->file->buff->s[e->file->buff->len++] = inst->p[i]
					>> inst->s[i] * 8;
				if (e->file->buff->len >= BS_ASM)
					add_buff(e);
			}
	}
	if (!inst->error)
		e->file->i = inst->index;
}

void	parse_inst(t_env *e, char *str)
{
	t_inst	inst;
	char	*tmp;

	inst = (t_inst){.ocp = 0}; // verifier norme
	if (e->file->error < MAX_ERROR && (inst.op = get_curr_inst(str)) <= 16)
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
	else if (e->file->error < MAX_ERROR)
	{
		inst.error++;
		error_unknow_inst(e, str);
	}
	write_inst(e, &inst);
}
