/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_1_5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:25:38 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/13 13:46:12 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		live(t_env *e, int *pc, t_proc *ptr)
{
	int		player_nb;
	int		j;

	*pc += 1;
	j = -1;
	e->nb_live++;
	ptr->live++;
	player_nb = param_sum(e, *pc, 4);
	if (player_nb == ptr->owner)
		e->last_live = player_nb;
	while (++j < e->nb_champ)
		if (player_nb == e->champs[j].id)
			e->last_live = e->champs[j].id;
	*pc += 4;
}

void		ld(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		addr;
	int		value;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	addr = param_value(e, check, 1, ptr);
	error = (!check.p1 || check.p2 != 16 || check.p3) ? 1 : 0;
	if (!error && check_reg(e->mem[(*pc + 2 + check.s1) % MEM_SIZE]))
	{
		if (check.s1 == 1 && check_reg(e->mem[(*pc + 2) % MEM_SIZE]))
		{
			value = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 2)
		{
			value = param_sum(e, *pc + (addr % IDX_MOD), REG_SIZE);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 4)
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = addr;
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
	if (check.s1 == 4)
		ptr->carry = (!error && !addr);
	else
		ptr->carry = (!error && !value);
}

void		st(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		addr;
	int		value;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (check.p1 != 64 || !check.p2 || check.p3) ? 1 : 0;
	addr = param_value(e, check, 2, ptr);
	if (!error && check_reg(e->mem[(*pc + 2) % MEM_SIZE]))
	{
		if (check.s2 == 1 && check_reg(e->mem[(*pc + 2 + check.s1) % MEM_SIZE]))
		{
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] =
				ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
		}
		else if (check.s2 == 2)
		{
			value = param_sum(e, *pc + (addr % IDX_MOD), REG_SIZE);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s2 == 4)
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = addr;
	}
	*pc += 2 + check.s1 + check.s2;
}

void		add(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (check.p1 != 64 || check.p2 != 16 || check.p3 != 4) ? 1 : 0;
	if (!check_reg(e->mem[(*pc + 2) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 3) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 4) % MEM_SIZE]))
		error = 1;
	if (!error)
	{
		v1 = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
		v2 = ptr->r[e->mem[(*pc + 3) % MEM_SIZE]];
		ptr->r[e->mem[(*pc + 4) % MEM_SIZE]] = v1 + v2;
	}
	ptr->carry = (!error && (!v1 || !v2)) ? 1 : 0;
	*pc += 5;
}

void		sub(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (check.p1 != 64 || check.p2 != 16 || check.p3 != 4) ? 1 : 0;
	if (!check_reg(e->mem[(*pc + 2) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 3) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 4) % MEM_SIZE]))
		error = 1;
	if (!error)
	{
		v1 = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
		v2 = ptr->r[e->mem[(*pc + 3) % MEM_SIZE]];
		ptr->r[e->mem[(*pc + 4) % MEM_SIZE]] = v1 - v2;
	}
	ptr->carry = (!error && (!v1 || !v2)) ? 1 : 0;
	*pc += 5;
}
