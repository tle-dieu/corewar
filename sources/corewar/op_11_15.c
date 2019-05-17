/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_11_15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:22:32 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/17 12:49:16 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		sti(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
			g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error)
	{
		param_value(e, &check, ptr, 1);
		reg = e->mem[(*pc + 2) % MEM_SIZE];
		if (!check.error && reg > 0 && reg < 17)
		{
			e->v.color = e->visu ? ptr->color : 0;
			insert(e, (*pc + ((check.v[1] + check.v[2]) % IDX_MOD))
				% MEM_SIZE, (void*)&ptr->r[reg], REG_SIZE);
		}
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}

void		op_fork(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = param_sum(e, *pc + 1, 2);
	create_new_process(e, (*pc + (addr % IDX_MOD)) % MEM_SIZE, ptr);
	*pc += 3;
}

void		lld(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	short	addr;
	int		reg;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
			g_op_tab[ptr->op - 1].dir_size, ptr->op);
	reg = e->mem[(*pc + 2 + check.s[0]) % MEM_SIZE];
	if (!check.error && reg > 0 && reg < 17)
	{
		if (check.s[0] == 2)
		{
			addr = param_sum(e, *pc + 2, 2);
			ptr->r[reg] = param_sum(e, *pc + addr % MEM_SIZE, REG_SIZE);
		}
		else if (check.s[0] == 4)
			ptr->r[reg] = param_sum(e, ptr->pc + 2, check.s[0]);
		ptr->carry = !ptr->r[reg];
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}

void		lldi(t_env *e, int *pc, t_proc *ptr)
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
			ptr->r[reg] = param_sum(e,
					(*pc + check.v[0] + check.v[1]) % MEM_SIZE, REG_SIZE);
	}
	*pc = *pc + 2 + check.s[0] + check.s[1] + check.s[2];
}

void		lfork(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = param_sum(e, *pc + 1, 2);
	create_new_process(e, (*pc + addr) % MEM_SIZE, ptr);
	*pc += 3;
}
