/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 17:17:09 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/18 02:06:16 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		get_params(t_env *e, t_inst *inst, char *begin, char *tmp)
{
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
		tmp += ft_strcspn(tmp + ft_strspn(tmp, SPACES), SEPARATOR_CHAR);
		inst->i++;
		if (!*tmp || e->file->error >= MAX_ERROR)
			break ;
		if (e->file->error < MAX_ERROR && *tmp == ','
			&& !tmp[ft_strspn(tmp, SPACES)] && ++inst->error)
			basic_error(e, tmp - 1, "expected parameter after ','\n", 0);
		tmp++;
	}
}

void		check_params(t_env *e, char *str, t_inst *inst)
{
	char	*tmp;
	char	*begin;

	tmp = str;
	begin = tmp;
	inst->index = e->file->i + g_op_tab[inst->op - 1].ocp + 1;
	get_params(e, inst, begin, tmp);
	if (e->file->error < MAX_ERROR
		&& inst->i != g_op_tab[inst->op - 1].nb_param && ++inst->error)
		error_nb_param(e, begin, inst->i, g_op_tab[inst->op - 1].nb_param);
}
