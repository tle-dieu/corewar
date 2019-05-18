/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 12:17:12 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/18 02:34:33 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	error_nb_param(t_env *e, char *str, int have, int should_have)
{
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, e->file->last->y, str - e->file->last->s + 1);
	ft_dprintf(2, "'%s'\n", str);
	if (have > should_have)
		ft_dprintf(2, "too many parameter, have %d parameter expected %d\n",
			have, should_have);
	else
		ft_dprintf(2, "missing parameter, have %d parameter expected %d\n",
			have, should_have);
	err_pointer(e->tty2, e->file->last->s, str + (!*str));
	err_wave(e->tty2, str, param_strrspn(str, SPACES, 0));
	ft_dprintf(2, "\n");
}

void	print_expected_types(t_inst *inst, int type, int op_type)
{
	char	*types[4];

	types[0] = REGISTER;
	types[1] = DIRECT;
	types[2] = "";
	types[3] = INDIRECT;
	ft_dprintf(2, "parameter[%d] type is %s, expected type(s) (",
		inst->i + 1, types[type - 1]);
	if (op_type & T_IND)
		ft_dprintf(2, INDIRECT) && ((op_type & T_DIR) || (op_type & T_REG))
		&& ft_dprintf(2, " | ");
	if (op_type & T_DIR)
		ft_dprintf(2, DIRECT) && (op_type & T_DIR) && (op_type & T_REG)
			&& ft_dprintf(2, " | ");
	if (op_type & T_REG)
		ft_dprintf(2, REGISTER);
}

void	error_param_type(t_env *e, t_inst *inst, char *str)
{
	int			type;
	int			op_type;

	type = inst->t[inst->i] + (inst->t[inst->i] == 3);
	op_type = g_op_tab[inst->op - 1].param[inst->i];
	if (!(g_op_tab[inst->op - 1].nb_param > inst->i && !(type & op_type)))
		return ;
	inst->error++;
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, e->file->last->y, str - e->file->last->s + 1);
	print_expected_types(inst, type, op_type);
	ft_dprintf(2, ") for instruction '%s'\n", g_op_tab[inst->op - 1].label);
	err_pointer(e->tty2, e->file->last->s, str);
	err_wave(e->tty2, str, param_strrspn(str, SPACES, ','));
	ft_dprintf(2, "\n");
}

int		error_register_nb(t_env *e, char *str, int nb)
{
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, e->file->last->y, str - e->file->last->s + 1);
	ft_dprintf(2, "register index must be between 1 and 16 have %d\n", nb);
	err_pointer(e->tty2, e->file->last->s, str);
	err_wave(e->tty2, str, ft_strspn(str, "0123456789") - 1);
	ft_dprintf(2, "\n");
	return (-1);
}

void	error_unknow_inst(t_env *e, char *str)
{
	int len;

	len = ft_strcspn(str, END_PARAM);
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, e->file->last->y, str - e->file->last->s + 1);
	ft_dprintf(2, "unknow instruction '%.*s'\n", len, str);
	err_pointer(e->tty2, e->file->last->s, str);
	err_wave(e->tty2, str, len - 1);
	ft_dprintf(2, "\n");
}
