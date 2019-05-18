/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 15:03:50 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/18 02:43:53 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		is_direct(t_env *e, char *str, t_inst *inst)
{
	char	*tmp;

	tmp = str;
	if (*tmp == DIRECT_CHAR)
	{
		++tmp;
		inst->s[inst->i] = g_op_tab[inst->op - 1].dir_size ? 2 : 4;
		inst->t[inst->i] = 2;
		if (*tmp == LABEL_CHAR)
		{
			if (label_is_good(e, tmp + 1))
				get_label_call(e, inst, tmp + 1, inst->i);
			else
				inst->error++;
		}
		else
		{
			if (is_a_number(e, tmp))
				inst->p[inst->i] = inst_atoi(tmp);
			else
				return (++inst->error && !(inst->t[inst->i] = 0));
		}
	}
	return (*str == DIRECT_CHAR);
}

int		is_indirect(t_env *e, char *str, t_inst *inst)
{
	char	*tmp;

	tmp = str;
	inst->s[inst->i] = IND_SIZE;
	inst->t[inst->i] = 3;
	if (*tmp == LABEL_CHAR)
	{
		if (label_is_good(e, tmp + 1))
			get_label_call(e, inst, tmp + 1, inst->i);
		else
			inst->error++;
	}
	else
	{
		if (is_a_number(e, tmp))
			inst->p[inst->i] = inst_atoi(tmp);
		else
		{
			inst->error++;
			return (inst->t[inst->i] = 0);
		}
	}
	return (1);
}

int		is_reg(t_env *e, char *str, t_inst *inst)
{
	char	*tmp;

	tmp = str;
	if (*tmp == 'r')
	{
		tmp++;
		inst->s[inst->i] = 1;
		inst->t[inst->i] = 1;
		if (is_valid_register(e, tmp))
			inst->p[inst->i] = inst_atoi(tmp);
		else
			inst->error++;
		if (e->file->error < MAX_ERROR
			&& (inst->p[inst->i] < 0 || inst->p[inst->i] > REG_NUMBER))
		{
			error_register_nb(e, tmp, inst->p[inst->i]);
			inst->error++;
		}
		return (1);
	}
	return (0);
}
