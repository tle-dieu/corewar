/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_6_10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:20:02 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/01 12:53:46 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		and(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		v1;
	int		v2;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		v1 = param_value_1(e, check, ptr, 0);
		v2 = param_value_2(e, check, ptr, 0);
		reg = e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			ptr->r[reg] = v1 & v2;
			ptr->carry = !ptr->r[reg];
		}
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		or(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		v1;
	int		v2;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		v1 = param_value_1(e, check, ptr, 0);
		v2 = param_value_2(e, check, ptr, 0);
		reg = e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			ptr->r[reg] = v1 | v2;
			ptr->carry = (!ptr->r[reg]);
		}
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		xor(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		v1;
	int		v2;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		v1 = param_value_1(e, check, ptr, 0);
		v2 = param_value_2(e, check, ptr, 0);
		reg = e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			ptr->r[reg] = v1 ^ v2;
			ptr->carry = (!ptr->r[reg]);
		}
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
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
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		v1 = param_value_1(e, check, ptr, 1);
		v2 = param_value_2(e, check, ptr, 1);
		reg = param_sum(e, (*pc + 2 + check.s1 + check.s2)
			% MEM_SIZE, check.s3);
		if (!check.error && reg > 0 && reg < 17)
			ptr->r[reg] = param_sum(e, (*pc + v1 + v2) % MEM_SIZE, REG_SIZE);
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}
