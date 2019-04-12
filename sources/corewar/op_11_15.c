/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_11_15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:22:32 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/12 21:16:04 by matleroy         ###   ########.fr       */
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
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p1 == 64 && check.p2 && check.p3 < 12)
	{
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		if (check.p2 == 16 && (reg = check_reg(p)))
			sum += ptr->r[p];
		else if (reg && check.p2 > 32)
			sum += (short)e->mem[*pc + (p % IDX_MOD)];
		else if (reg)
			sum += (short)p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		sum += reg && check.p3 != 8 && check_reg(p) ? ptr->r[p] : (short)p;
		p = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (reg)
			insert(e, (*pc + sum) % MEM_SIZE,
					(void*)&ptr->r[p], 4);
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
}

void		op_fork(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = (short)param_sum(e, *pc + 1, 2);
	if (!(create_new_process(e, (*pc + (addr % IDX_MOD)) % MEM_SIZE, ptr)))
		freedom(e);
	*pc += 3;
}

void		lld(t_env *e, int *pc, t_proc *ptr)
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
			value = param_sum(e, *pc + addr, REG_SIZE);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 4)
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = addr;
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
	ptr->carry = (!error && !value) ? 1 : 0;
}

void		lldi(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		sum;
	int		p;
	int		reg;

	reg = 1;
	sum = 0;
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p3 == 4 && check.p2 && check.p2 <= 32 && check.p3)
	{
		p = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (check.p1 == 64 && (reg = check_reg(p)))
			sum += ptr->r[p];
		else if (check.p1 > 128 && reg)
			sum += e->mem[(*pc + p) % MEM_SIZE];
		else if (reg)
			sum += p;
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		sum += (check.p2 == 16 && reg && check_reg(p)) ? ptr->r[p] : p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		if (check.p3 == 4 && reg && check_reg(p))
			ptr->r[p] = sum;
		ptr->carry = (reg && !sum);
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
}

void		lfork(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = (short)param_sum(e, *pc + 1, 2);
	if (!(create_new_process(e, *pc + addr, ptr)))
		freedom(e);
	*pc += 3;
}
