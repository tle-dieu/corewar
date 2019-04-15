/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_6_10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:20:02 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/15 18:14:48 by acompagn         ###   ########.fr       */
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
	error = (check.error || !check.p1 || !check.p2 || check.p3 != 4);
	if (!error)
	{
		v1 = param_value(e, check, 1, ptr);
		v2 = param_value(e, check, 2, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 & v2;
	}
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
		*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
	if (!error)
		ptr->carry = (!ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]]);
}

void		or(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (check.error || !check.p1 || !check.p2 || check.p3 != 4);
	if (!error)
	{
		v1 = param_value(e, check, 1, ptr);
		v2 = param_value(e, check, 2, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 | v2;
	}
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
		*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
	if (!error)
		ptr->carry = (!ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]]);
}

void		xor(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (check.error || !check.p1 || !check.p2 || check.p3 != 4);
	if (!error && check_reg(e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]))
	{
		v1 = param_value(e, check, 1, ptr);
		v2 = param_value(e, check, 2, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 ^ v2;
	}
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
		*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
	if (!error)
		ptr->carry = (!ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]]);
}

void		zjmp(t_env *e, int *pc, t_proc *ptr)
{
	short	addr;

	addr = param_sum(e, *pc + 1, 2);
	if (ptr->carry == 1)
	{
	//	ft_printf("zjump from %d ", *pc);
		*pc = (*pc + (addr % IDX_MOD)) % MEM_SIZE;
	//	ft_printf("to %d\n", *pc);
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
	int		error;

	reg = 1;
	sum = 0;
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (!check.error && check.p3 == 4 && check.p2 <= 32 && check.p1)
	{
		error = 0;
		p = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (check.p1 == 64 && (reg = check_reg(p)))
			sum += ptr->r[p];
		else if (reg && check.p1 > 128)
			sum += e->mem[(*pc + (p % IDX_MOD)) % MEM_SIZE];
		else if (reg)
			sum += p;
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		if (check.p2 == 16 && reg && check_reg(p))
			sum += ptr->r[p];
		else if (reg)
			sum += p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		if (check.p3 == 4 && reg && check_reg(p))
			ptr->r[p] = param_sum(e, (*pc + sum) % MEM_SIZE, 4);
	}
	else
		error = 1;
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
		*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}
