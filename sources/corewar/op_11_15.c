/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_11_15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:22:32 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/11 20:54:40 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		sti(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		sum;
	int		err;
	int		m;

	err = 0;
	m = 0;
	sum = 0;
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p1 == 64 && check.p2 && check.p3 < 12)
	{
		m = 2 + check.s1;
		if (check.p2 == 16)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.s2)];
		else if (check.p2 > 32)
			sum += e->mem[*pc + (param_sum(e,
						(*pc + m) % MEM_SIZE, check.s2) % IDX_MOD)];
		else
			sum += param_sum(e, (*pc + m) % MEM_SIZE, check.s2);
		m = 2 + check.s1 + check.s2;
		if (!err && check.p3 == 8)
			sum += param_sum(e, (*pc + m) % MEM_SIZE, check.s3);
		else if (!err && check.p3 == 4)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.s3)];
		else
			err = 1;
		m = 2;
		insert(e, (*pc + (sum % IDX_MOD)) % MEM_SIZE,
			(void*)&ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, 1)], 4);
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
}

void		op_fork(t_env *e, int *pc, t_proc *ptr)
{
	int		ind;

	ind = param_sum(e, *pc + 1, 2);
	if (!(create_new_process(e, *pc + (ind % IDX_MOD), ptr)))
		freedom(e);
	*pc += 2;
}

void		lld(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		addr;
	int		value;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	addr = find_param_value(e, check, 1, pc, ptr);
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
	int		m;
	int		err;

	err = 0;
	sum = 0;
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p3 == 4 && check.p2 <= 32 && check.p3)
	{
		m = 2;
		if (check.p1 == 4)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.p1)];
		else if (check.p1 > 8)
			sum += e->mem[(*pc + param_sum(e,
						(*pc + m) % MEM_SIZE, IND_SIZE)) % MEM_SIZE];
		else
			sum += param_sum(e, (*pc + m) % MEM_SIZE, check.p3);
		m = 2 + check.s1;
		if (!err && check.p2 == 16)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.p2)];
		else if (!err && check.p2 == 32)
			sum += param_sum(e, (*pc + m) % MEM_SIZE, check.p2);
		else
			err = 1;
		m = 2 + check.s1 + check.s2;
		if (!err && check.p3 == 64)
			ptr->r[param_sum(e, *pc % MEM_SIZE, check.p3)] = sum;
		else
			err = 1;
		ptr->carry = !sum;
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
}

void		lfork(t_env *e, int *pc, t_proc *ptr)
{
	int		ind;

	ind = param_sum(e, *pc + 1, 2);
	if (!(create_new_process(e, *pc + ind, ptr)))
		freedom(e);
	*pc += 2;
}
