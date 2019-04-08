/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_codes2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:15:29 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/08 21:01:11 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		ldi(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int sum;
	int m;
	int err;

	err = 0;
	m = 0;
	sum = 0;
	ft_printf("in ldi :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p1 && check.p2 && check.p3)
	{
		m = 1;
		if (check.p3 == 64 && check.p2 <= 32)
		{
			if (check.p3 == 4)
				sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.p3)];
			else if (check.p3 > 4)
				sum += e->mem[*pc + param_sum(e, (*pc + m) % MEM_SIZE, check.p3)];
			else
				sum += param_sum(e, (*pc + m) % MEM_SIZE, check.p3);
			m += check.s3;
			if (!err && check.p2 == 32)
				sum += param_sum(e, (*pc + m) % MEM_SIZE, check.p2);
			else if (!err && check.p2 > 32)
				sum += e->mem[*pc + param_sum(e, (*pc + m) % MEM_SIZE, check.p2)];
			else
				err = 1;
			if (!err && check.p1 == 64)
				ptr->r[param_sum(e, *pc % MEM_SIZE, check.p1)] = sum;
			else
				err = 1;
			ptr->carry = !sum;
		}
		move_pc(check, pc, 1);
	}
}

//fait

void		sti(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int sum;
	int err;
	int m;

	err = 0;
	m = 0;
	sum = 0;
	ft_printf("in sti :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p1 == 64 && check.p2  && check.p3 < 12 && check.p3)
	{
		ft_printf("OK ");
		m = 1;
		if (check.p2 == 4)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.p2)];
		else if (check.p2 > 4)
			sum += e->mem[*pc + param_sum(e, (*pc + m) % MEM_SIZE, check.p2)];
		else
			sum += param_sum(e, (*pc + m) % MEM_SIZE, check.p2);
		if (!err && check.p3 == 32)
			sum += param_sum(e, (*pc + m) % MEM_SIZE, check.p3);
		else if (!err && check.p3 > 32)
			sum += e->mem[*pc + param_sum(e, (*pc + m) % MEM_SIZE, check.p3)];
		else
			err = 1;
		if ((e->mem[*pc + m] = ptr->r[param_sum(e, *pc % MEM_SIZE, 1)]))
			ptr->carry = 0;
		else
			ptr->carry = 1;
	}
	else
		ft_printf("ERROR ");
	move_pc(check, pc, 1);
}

void		op_fork(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in op_fork :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void		lld(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in lld :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void		lldi(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in lldi :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void		lfork(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in lfork :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

// FAIT!
void		aff(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;

	ft_printf("in aff :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (check.p1 != 64 || check.p2 || check.p3)
		ft_printf("ERROR :: wrong args, must have one register\n");
	else
	{
		*pc += 2;
		if (ptr)
			ft_printf("%c", ptr->r[e->mem[*pc % MEM_SIZE]] % 256);
	}
}
