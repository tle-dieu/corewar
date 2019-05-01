/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_11_15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:22:32 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/01 12:53:42 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		sti(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;
	int		v2;
	int		v3;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
			g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		v2 = param_value_2(e, check, ptr, 1);
		v3 = param_value_3(e, check, ptr, 1);
		reg = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (!check.error && reg > 0 && reg < 17)
		{
			e->v.color = e->visu ? ptr->color : 0;
			insert(e, (*pc + v2 + v3) % MEM_SIZE,
				(void*)&ptr->r[reg], REG_SIZE);
		}
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		op_fork(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = param_sum(e, *pc + 1, 2);
	if (!(create_new_process(e, (*pc + (addr % IDX_MOD)) % MEM_SIZE, ptr)))
		freedom(e);
	*pc += 3;
}

void		lld(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	short	addr;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
			g_op_tab[ptr->op - 1].dir_size, ptr->op);
	reg = e->mem[(*pc + 2 + check.s1) % MEM_SIZE];
	if (!check.error && reg > 0 && reg < 17)
	{
		if (check.s1 == 2)
		{
			addr = param_sum(e, *pc + 2, 2);
			ptr->r[reg] = param_sum(e, *pc + addr % MEM_SIZE, REG_SIZE);
		}
		else if (check.s1 == 4)
			ptr->r[reg] = param_sum(e, ptr->pc + 2, check.s1);
		ptr->carry = !ptr->r[reg];
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		lldi(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
			g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		v1 = param_value_1(e, check, ptr, 0);
		v2 = param_value_2(e, check, ptr, 0);
		reg = param_sum(e, (*pc + 2 + check.s1 + check.s2)
			% MEM_SIZE, check.s3);
		if (!check.error && reg > 0 && reg < 17)
			ptr->r[reg] = param_sum(e, (*pc + v1 + v2) % MEM_SIZE, REG_SIZE);
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		lfork(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = param_sum(e, *pc + 1, 2);
	if (!(create_new_process(e, (*pc + addr) % MEM_SIZE, ptr)))
		freedom(e);
	*pc += 3;
}
