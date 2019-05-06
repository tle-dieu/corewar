/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 17:17:09 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/06 17:51:13 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
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
			{
				inst->error++;
				return (!(inst->t[inst->i] = 0));
			}
		}
		return (1);
	}
	return (0);
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

int		is_valid_register(t_env *e, char *str)
{
	char	*tmp;
	int		err;

	err = 0;
	tmp = str;
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != *SEPARATOR_CHAR && *tmp && !ft_strchr(SPACES, *tmp))
		err += basic_error(e, tmp, "illegal character for label\n", 0);
	tmp += ft_strcspn(tmp, SPACES",");
	tmp += ft_strspn(tmp, SPACES);
	if (e->file->error < MAX_ERROR && *tmp && *tmp != *SEPARATOR_CHAR)
		err += basic_error(e, tmp, "unexpected expression after parameter\n",
				ft_strcspn(tmp, SEPARATOR_CHAR) - 1);
	return (!err);
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
		if (e->file->error < MAX_ERROR && (inst->p[inst->i] < 0 || inst->p[inst->i] > REG_NUMBER))
		{
			error_register_nb(e, tmp, inst->p[inst->i]);
			inst->error++;
		}
		return (1);
	}
	return (0);
}

int		check_params(t_env *e, char *str, t_inst *inst)
{
	char	*tmp;
	char	*begin;

	tmp = str;
	begin = tmp;
	inst->index = e->file->i + g_op_tab[inst->op - 1].ocp + 1;
	while (*tmp && e->file->error < MAX_ERROR)
	{
		tmp += ft_strspn(tmp, SPACES);
		if (g_op_tab[inst->op - 1].nb_param > inst->i)
		{
			begin = tmp;
			is_direct(e, tmp, inst) || is_reg(e, tmp, inst)
			|| is_indirect(e, tmp, inst);
			if (e->file->error < MAX_ERROR && inst->t[inst->i])
			{
				error_param_type(e, inst, begin);
				inst->index += inst->s[inst->i];
			}
		}
		tmp += ft_strspn(tmp, SPACES);
		tmp += ft_strcspn(tmp, SEPARATOR_CHAR);
		inst->i++;
		if (!*tmp || e->file->error >= MAX_ERROR)
			break ;
		if (*tmp++ == ',' && !tmp[ft_strspn(tmp, SPACES)])
		{
			basic_error(e, tmp - 1, "expected parameter after ','\n", 0);
			inst->error++;
		}
	}
	if (e->file->error >= MAX_ERROR)
		return (-1);
	if (inst->i != g_op_tab[inst->op - 1].nb_param)
	{
		inst->error++;
		error_nb_param(e, begin, inst->i, g_op_tab[inst->op - 1].nb_param);
	}
	return (1);
}
