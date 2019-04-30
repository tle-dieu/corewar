/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_6_10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:20:02 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/30 15:55:21 by acompagn         ###   ########.fr       */
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
	reg = e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE];
	if (reg < 0 || reg > 17)
		check.error = 1;
	if (!check.error)
	{
		v1 = param_value(e, check, 1, ptr);
		v2 = param_value(e, check, 2, ptr);
		ptr->r[reg] = v1 & v2;
		ptr->carry = !ptr->r[reg];
		ft_printf("ptr->r[%d] = %d\n", reg, ptr->r[reg]);
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
	reg = e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE];
	if (reg < 0 || reg > 17)
		check.error = 1;
	if (!check.error)
	{
		v1 = param_value(e, check, 1, ptr);
		v2 = param_value(e, check, 2, ptr);
		ptr->r[reg] = v1 | v2;
		ptr->carry = (!ptr->r[reg]);
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
	reg = e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE];
	if (reg < 0 || reg > 17)
		check.error = 1;
	if (!check.error)
	{
		v1 = param_value(e, check, 1, ptr);
		v2 = param_value(e, check, 2, ptr);
		ptr->r[reg] = v1 ^ v2;
		ptr->carry = (!ptr->r[reg]);
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
	int		sum;
	int		p;
	int		reg;

	reg = 1;
	sum = 0;
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		p = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (check.p1 == 64 && (reg = check_reg(p)))
			sum += ptr->r[p];
		else if (reg)
			sum += check.p1 > 128 ? param_sum(e,
				(*pc + (p % IDX_MOD)) % MEM_SIZE, 2) : p;
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		if (reg)
			sum += (check.p2 == 16 && check_reg(p)) ? ptr->r[p] : p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		if (reg && p > 0 && p < 17)
			ptr->r[p] = param_sum(e, (*pc + sum) % MEM_SIZE, REG_SIZE);
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}
