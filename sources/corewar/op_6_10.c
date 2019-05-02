/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_6_10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:20:02 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/02 15:01:14 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		and(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		param_value(e, &check, ptr, 0);
		reg = e->mem[(*pc + 2 + check.s[0] + check.s[1]) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			ptr->r[reg] = check.v[0] & check.v[1];
			ptr->carry = !ptr->r[reg];
		}
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}

void		or(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		param_value(e, &check, ptr, 0);
		reg = e->mem[(*pc + 2 + check.s[0] + check.s[1]) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			ptr->r[reg] = check.v[0] | check.v[1];
			ptr->carry = !ptr->r[reg];
		}
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}

void		xor(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		param_value(e, &check, ptr, 0);
		reg = e->mem[(*pc + 2 + check.s[0] + check.s[1]) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			ptr->r[reg] = check.v[0] ^ check.v[1];
			ptr->carry = !ptr->r[reg];
		}
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}

void		zjmp(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = param_sum(e, *pc + 1, 2);
	if (ptr->carry == 1)
	{
		*pc = (*pc + (addr % IDX_MOD)) % MEM_SIZE;
		if (*pc < 0)
			*pc = *pc % MEM_SIZE + MEM_SIZE;
	}
	else
		*pc += 3;
}

void		ldi(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		param_value(e, &check, ptr, 1);
		reg = e->mem[(*pc + 2 + check.s[0] + check.s[1]) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
			ptr->r[reg] = param_sum(e,
					(*pc + check.v[0] + check.v[1]) % MEM_SIZE, REG_SIZE);
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}
