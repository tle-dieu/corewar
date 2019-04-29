/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_11_15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:22:32 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/29 13:41:12 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		sti(t_env *e, int *pc, t_proc *ptr)
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
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		if (check.p2 == 16 && (reg = check_reg(p)))
			sum += ptr->r[p];
		else if (reg)
			sum += check.p2 > 32 ? e->mem[(*pc + (p % IDX_MOD)) % MEM_SIZE] : p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		sum += (reg && check.p3 != 8 && check_reg(p)) ? ptr->r[p] : p;
		p = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (reg)
		{
			e->v.color = e->visu ? ptr->color : 0;
			insert(e, (*pc + sum) % MEM_SIZE, (void*)&ptr->r[p], 4);
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
	int		value;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	addr = param_value(e, check, 1, ptr);
	if (!check.error && check_reg(e->mem[(*pc + 2 + check.s1) % MEM_SIZE]))
	{
		if (check.s1 == 1 && check_reg(e->mem[(*pc + 2) % MEM_SIZE]))
		{
			value = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 2)
		{
			value = param_sum(e, *pc + 3, 1);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 4)
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = addr;
		ptr->carry = !ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]];
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		lldi(t_env *e, int *pc, t_proc *ptr)
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
		else if (reg && check.p1 > 128)
			sum += e->mem[(*pc + p) % MEM_SIZE];
		else if (reg)
			sum += p;
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		if (reg)
			sum += (check.p2 == 16 && check_reg(p)) ? ptr->r[p] : p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		if (check.p3 == 4 && reg && check_reg(p))
			ptr->r[p] = param_sum(e, (*pc + sum) % MEM_SIZE, 4);
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
