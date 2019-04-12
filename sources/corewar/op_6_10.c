/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_6_10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:20:02 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/11 20:52:09 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		and(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (!check.p1 || !check.p2 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		v1 = find_param_value(e, check, 1, pc, ptr);
		v2 = find_param_value(e, check, 2, pc, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 & v2;
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
	if (!error)
		ptr->carry = (!v1 || !v2) ? 1 : 0;
}

void		or(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (!check.p1 || !check.p2 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		v1 = find_param_value(e, check, 1, pc, ptr);
		v2 = find_param_value(e, check, 2, pc, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 | v2;
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
	if (!error)
		ptr->carry = (!v1 || !v2) ? 1 : 0;
}

void		xor(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (!check.p1 || !check.p2 || check.p3 != 4) ? 1 : 0;
	if (!error && check_reg(e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]))
	{
		v1 = find_param_value(e, check, 1, pc, ptr);
		v2 = find_param_value(e, check, 2, pc, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 ^ v2;
	}
	*pc += 2 + check.s1 + check.s2 + check.s3;
	if (!error)
		ptr->carry = (!v1 || !v2) ? 1 : 0;
}

void		zjmp(t_env *e, int *pc, t_proc *ptr)
{
	int		addr;

	if (!(e->mem[(*pc + 1) % MEM_SIZE]))
		addr = param_sum(e, *pc + 1, 2);
	else
	{
		addr = e->mem[(*pc + 1) % MEM_SIZE] - e->mem[(*pc + 2) % MEM_SIZE];
		addr = -addr - 1;
	}
	if (ptr->carry == 0)
		*pc = (*pc + (addr % IDX_MOD)) % MEM_SIZE;
	else
		*pc += 3;
}

void		ldi(t_env *e, int *pc, t_proc *ptr)
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
						(*pc + m) % MEM_SIZE, IND_SIZE) % IDX_MOD) % MEM_SIZE];
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
