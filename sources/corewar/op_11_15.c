/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_11_15.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:22:32 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/15 18:14:50 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		sti(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		sum;
	int		p;
	int		reg;
	int		error;

	reg = 1;
	sum = 0;
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (!check.error && check.p1 == 64 && check.p2 && check.p3 < 12)
	{
		error = 0;
		p = param_sum(e, (*pc + 2 + check.s1) % MEM_SIZE, check.s2);
		if (check.p2 == 16 && (reg = check_reg(p)))
			sum += ptr->r[p];
		else if (reg && check.p2 > 32)
			sum += e->mem[*pc + (p % IDX_MOD)];
		else if (reg)
			sum += p;
		p = param_sum(e, (*pc + 2 + check.s1 + check.s2) % MEM_SIZE, check.s3);
		sum += reg && check.p3 != 8 && check_reg(p) ? ptr->r[p] : p;
		p = param_sum(e, (*pc + 2) % MEM_SIZE, check.s1);
		if (reg)
			insert(e, (*pc + (sum % IDX_MOD)) % MEM_SIZE,
					(void*)&ptr->r[p], 4);
	}
	else
		error = 1;
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
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
	int		error;
	short	addr;
	int		value;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	addr = param_value(e, check, 1, ptr);
	error = (check.error || !check.p1 || check.p2 != 16 || check.p3);
	if (!error && check_reg(e->mem[(*pc + 2 + check.s1) % MEM_SIZE]))
	{
		if (check.s1 == 1 && check_reg(e->mem[(*pc + 2) % MEM_SIZE]))
		{
			value = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
	//		ft_printf("value %d addr %d\n", value, addr);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 2)
		{
			value = param_sum(e, *pc + addr, REG_SIZE);
	//		ft_printf("value = %d\n", value);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = value;
		}
		else if (check.s1 == 4)
		{
	//		ft_printf("addr = %d\n", addr);
			ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]] = addr;
		}
	}
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
		*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
	if (!error)
	{
		if (check.s1 == 4)
			ptr->carry = !addr;
		else
			ptr->carry = !value;
	}
}

void		lldi(t_env *e, int *pc, t_proc *ptr)
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
			sum += e->mem[(*pc + p) % MEM_SIZE];
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
		ptr->carry = (reg && !sum);
	}
	else
		error = 1;
	if (ERROR_NOT)
		*pc = error ? *pc + 1 : *pc + 2 + check.s1 + check.s2 + check.s3;
	else
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
