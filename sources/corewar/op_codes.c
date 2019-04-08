/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_codes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:14:09 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/08 20:02:25 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int			param_sum(t_env *e, int *pc, int size)
{
	int		res;
	int		j;

	res = e->mem[*pc % MEM_SIZE];
	j = 1;
	while (--size)
	{
		res *= 256;
		res += e->mem[(*pc + j++) % MEM_SIZE];
	}
	return (res);
}

int			find_param_value(t_env *e, t_ocp check, int to_find, int *pc, t_proc *ptr)
{
	int		value;

	value = 0;
	if (to_find == 1)
	{
		if (check.s1 == 1)
			value = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
		else if (check.s1 == 2)
			value = e->mem[param_sum(e, pc + 2, check.s1)];
		else if (check.s1 == 4)
			value = param_sum(e, pc + 2, check.s1);
	}
	else if (to_find == 2)
	{
		if (check.s2 == 1)
			value = ptr->r[e->mem[(*pc + 2 + check.s1) % MEM_SIZE]];
		else if (check.s2 == 2)
			value = e->mem[param_sum(e, pc + 2 + check.s1, check.s2)];
		else if (check.s2 == 4)
			value = param_sum(e, pc + 2 + check.s1, check.s2);
	}
	return (value);
}

t_ocp		find_param_size(t_ocp check, int ocp, int on_two)
{
	if (ocp >= 192 || ocp < 64)
		check.s1 = (ocp >= 192) ? 2 : 0;
	else
	{
		if (ocp >= 128)
			check.s1 = on_two ? 2 : 4;
		else
			check.s1 = 1;
	}
	if (ocp - check.p1 >= 48 || ocp - check.p1 < 16)
		check.s2 = (ocp - check.p1 >= 48) ? 2 : 0;
	else
	{
		if (ocp - check.p1 >= 32)
			check.s2 = on_two ? 2 : 4;
		else
			check.s2 = 1;
	}
	if (check.p3 == 12 || check.p3 == 4)
		check.s3 = (check.p3 == 12) ? 2 : 1;
	else if (check.p3 == 8)
		check.s3 = on_two ? 2 : 4;
	return (check);
}

t_ocp		check_ocp(int ocp, int on_two)
{
	t_ocp	check;

	if (ocp >= 192 || ocp < 64)
		check.p1 = (ocp >= 192) ? 192 : 0;
	else
		if (ocp >= 128)
			check.p1 = (ocp >= 128) ? 128 : 64;
	if (ocp - check.p1 >= 48 || ocp - check.p1 < 16)
		check.p2 = (ocp - check.p1 >= 48) ? 48 : 0;
	else
		check.p2 = (ocp - check.p1 >= 32) ? 32 : 16;
	check.p3 = ocp - check.p1 - check.p2;
	return (find_param_size(check, ocp, on_two));
}

void		move_pc(t_ocp check, int *pc, int on_two)
{
	*pc += 2;
	if (check.p1 == 128)
		*pc = (on_two) ? *pc + 2 : *pc + 4;
	else if (check.p1 == 192 || check.p1 == 64)
		*pc = (check.p1 == 192) ? *pc + 2 : *pc + 1;
	if (check.p2 == 32)
		*pc = (on_two) ? *pc + 2 : *pc + 4;
	else if (check.p2 == 48 || check.p2 == 16)
		*pc = (check.p2 == 48) ? *pc + 2 : *pc + 1;
	if (check.p3 == 8)
		*pc = (on_two) ? *pc + 2 : *pc + 4;
	else if (check.p3 == 12 || check.p3 == 4)
		*pc = (check.p3 == 12) ? *pc + 2 : *pc + 1;
}

int			check_reg(int reg)
{
	if (reg > 0 && reg < 17)
		return (1);
	else
	{
		ft_printf("Unvalid register\n");
		return (0);
	}
}

void		live(t_env *e, int *pc, t_proc *ptr)
{
	int		player_nb;
	int		j;

	*pc += 1;
	j = -1;
	if (ptr)
	{
		player_nb = (e->mem[*pc % MEM_SIZE] + e->mem[(*pc + 1) % MEM_SIZE]
				+ e->mem[(*pc + 2) % MEM_SIZE]) * 256 + e->mem[(*pc + 3) % MEM_SIZE];
		if (player_nb == ptr->owner)
			ptr->live++;
		else
			while (++j < e->nb_champ)
				if (player_nb == e->champs[j].id)
					e->champs[j].alive = 1;
		ft_printf("live :: for player %d\n", player_nb);
	}
	*pc += 4;
}

void		ld(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		tmp;
	int		error;

	ft_printf("in ld ::");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	tmp = *pc + 1;
	if (!check.p1 || check.p2 != 16 || check.p3)
	{
		error = 1;
		ptr->carry = 0;
	}
	else
		error = 0;
	move_pc(check, pc, 0);
	if (!error && check_reg(e->mem[(*pc - 1) % MEM_SIZE]))
		ptr->r[e->mem[(*pc - 1) % MEM_SIZE]] = e->mem[tmp % MEM_SIZE];
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	!error ? ptr->carry = 1 : 1;
}

void		st(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		ind;

	ft_printf("in st ::");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	error = (check.p1 != 64 || (check.p2 != 16 || check.p2 != 48) || check.p3) ? 1 : 0;
	if (!error)
	{
		if (check.p2 == 16 && check_reg(e->mem[(*pc + 2) % MEM_SIZE])
				&& check_reg(e->mem[(*pc + 1) % MEM_SIZE]))
			ptr->r[e->mem[(*pc + 2) % MEM_SIZE]] = ptr->r[e->mem[(*pc + 1) % MEM_SIZE]];
		else if (check.p2 != 16)
		{
			ind = e->mem[(*pc + 2) % MEM_SIZE] * 256 + e->mem[(*pc + 3) % MEM_SIZE];
			if (check_reg(e->mem[(*pc + 1) % MEM_SIZE]))
				e->mem[ind] = ptr->r[e->mem[(*pc + 1) % MEM_SIZE]];
		}
	}
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	move_pc(check, pc, 0);
}

void		add(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;

	ft_printf("in add ::");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (check.p1 != 64 || check.p2 != 16 || check.p3 != 4)
		ptr->carry = 0;
	error = (check.p1 != 64 || check.p2 != 16 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		if (check_reg(e->mem[(*pc + 2) % MEM_SIZE])
				&& check_reg(e->mem[(*pc + 3) % MEM_SIZE])
				&& check_reg(e->mem[(*pc + 4) % MEM_SIZE]))
			ptr->r[e->mem[(*pc + 4) % MEM_SIZE]] = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]]
				+ ptr->r[e->mem[(*pc + 3) % MEM_SIZE]];
		else
			error = 1;
	}
	!error ? ptr->carry = 1 : 1; 
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	*pc += 5;
}

void		sub(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;

	ft_printf("in sub :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (check.p1 != 64 || check.p2 != 16 || check.p3 != 4)
		ptr->carry = 0;
	error = (check.p1 != 64 || check.p2 != 16 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		if (check_reg(e->mem[(*pc + 2) % MEM_SIZE])
				&& check_reg(e->mem[(*pc + 3) % MEM_SIZE])
				&& check_reg(e->mem[(*pc + 4) % MEM_SIZE]))
			ptr->r[e->mem[(*pc + 4) % MEM_SIZE]] = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]]
				- ptr->r[e->mem[(*pc + 3) % MEM_SIZE]];
		else
			error = 1;
	}
	!error ? ptr->carry = 1 : 0;
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	*pc += 5;
}

void		and(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	ft_printf("in and :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (!check.p1 || !check.p2 || check.p3 != 4)
		ptr->carry = 0;
	error = (!check.p1 || !check.p2 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		v1 = find_param_value(e, check, 1, pc, ptr);
		v2 = find_param_value(e, check, 2, pc, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 & v2;
	}
	move_pc(check, pc, 0);
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	!error ? ptr->carry = 1 : 0;
}

void		or(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	ft_printf("in or :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (!check.p1 || !check.p2 || check.p3 != 4)
		ptr->carry = 0;
	error = (!check.p1 || !check.p2 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		v1 = find_param_value(e, check, 1, pc, ptr);
		v2 = find_param_value(e, check, 2, pc, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 | v2;
	}
	move_pc(check, pc, 0);
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	!error ? ptr->carry = 1 : 0;
}

void		xor(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		error;
	int		v1;
	int		v2;

	ft_printf("in xor :: ");
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (!check.p1 || !check.p2 || check.p3 != 4)
		ptr->carry = 0;
	error = (!check.p1 || !check.p2 || check.p3 != 4) ? 1 : 0;
	if (!error)
	{
		v1 = find_param_value(e, check, 1, pc, ptr);
		v2 = find_param_value(e, check, 2, pc, ptr);
		ptr->r[e->mem[(*pc + 2 + check.s1 + check.s2) % MEM_SIZE]] = v1 ^ v2;
	}
	move_pc(check, pc, 0);
	error ? ft_printf(" ERROR") : ft_printf(" OK");
	!error ? ptr->carry = 1 : 0;
}

void		zjmp(t_env *e, int *pc, t_proc *ptr)
{
	ft_printf("in zjmp :: NW");
	*pc += 3;
	(void)ptr;
	(void)e;
}
