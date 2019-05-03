/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 17:17:09 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/02 14:31:14 by matleroy         ###   ########.fr       */
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
		}
		else
		{
			if (is_a_number(e, tmp))
				inst->p[inst->i] = inst_atoi(tmp);
			else
				return (!(inst->t[inst->i] = 0));
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
	}
	else
	{
		if (is_a_number(e, tmp))
			inst->p[inst->i] = inst_atoi(tmp);
		else
			return (inst->t[inst->i] = 0);
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
		if (inst->p[inst->i] < 0 || inst->p[inst->i] > REG_NUMBER)
			error_register_nb(e, tmp, inst->p[inst->i]);
		return (1);
	}
	return (0);
}

int		check_params(t_env *e, char *str, t_inst *inst)
{
	char	*tmp;
	char	*begin;

	tmp = str;
	inst->index = e->file->i + g_op_tab[inst->op - 1].ocp + 1;
	while (*tmp)
	{
		tmp += ft_strspn(tmp, SPACES);
		if (g_op_tab[inst->op - 1].nb_param >= inst->i)
			begin = tmp;
		if (g_op_tab[inst->op - 1].nb_param > inst->i)
			is_direct(e, tmp, inst) || is_reg(e, tmp, inst)
			|| is_indirect(e, tmp, inst);
		tmp += ft_strspn(tmp, SPACES);
		tmp += ft_strcspn(tmp, SEPARATOR_CHAR);
		if (inst->t[inst->i] && g_op_tab[inst->op - 1].nb_param > inst->i)
		{
			error_param_type(e, inst, begin);
			inst->index += inst->s[inst->i];
		}
		inst->i++;
		if (!*tmp)
			break ;
		if (*tmp++ == ',' && !tmp[ft_strspn(tmp, SPACES)])
			basic_error(e, tmp - 1, "expected parameter after ','\n", 0);
	}
	if (inst->i != g_op_tab[inst->op - 1].nb_param)
		error_nb_param(e, begin, inst->i, g_op_tab[inst->op - 1].nb_param);
	return (1);
}
