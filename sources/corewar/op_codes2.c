/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_codes2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 20:19:06 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/11 16:11:22 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void insert(t_env *e, int pc, void *ptr, int size)
{
	char *tmp;
	tmp = (char*)ptr;
	while (size--)
		e->mem[pc + size] = *tmp++;
}

void        sti(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		sum;
	int		err;
	int		m;

	err = 0;
	m = 0;
	sum = 0;
	ft_printf("in sti :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p1 == 64 && check.p2  && check.p3 < 12)
	{
		m = 2 + check.s1;
		if (check.p2 == 16)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.s2)];
		else if (check.p2 > 32)
			sum += e->mem[*pc + (param_sum(e, (*pc + m) % MEM_SIZE, check.s2) % IDX_MOD)];
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
		insert(e, (*pc + (sum %IDX_MOD)) % MEM_SIZE, (void*)&ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, 1)], 4);
	}
	else
		ft_printf("ERROR ");
	*pc += 2 + check.s1 + check.s2 + check.s3;
}

void        ldi(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp   check;
	int sum;
	int m;
	int err;

	err = 0;
	sum = 0;
	ft_printf("in ldi :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p3 == 4 && check.p2 <= 32 && check.p3)
	{
		m = 2;
		if (check.p1 == 4)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.p1)];
		else if (check.p1 > 8)
			sum += e->mem[(*pc + param_sum(e, (*pc + m) % MEM_SIZE, IND_SIZE) % IDX_MOD) % MEM_SIZE];
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

void        op_fork(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in fork :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void        lld(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in lld :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void        lldi(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp   check;
	int sum;
	int m;
	int err;

	err = 0;
	sum = 0;
	ft_printf("in lldi :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 1);
	if (check.p3 == 4 && check.p2 <= 32 && check.p3)
	{
		m = 2;
		if (check.p1 == 4)
			sum += ptr->r[param_sum(e, (*pc + m) % MEM_SIZE, check.p1)];
		else if (check.p1 > 8)
			sum += e->mem[(*pc + param_sum(e, (*pc + m) % MEM_SIZE, IND_SIZE)) % MEM_SIZE];
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

void        lldi(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in lldi :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void        lfork(t_env *e, int *i, t_proc *ptr)
{
	ft_printf("in lfork :: ");
	(void)e;
	(void)i;
	(void)ptr;
}

void        aff(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp   check;

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
