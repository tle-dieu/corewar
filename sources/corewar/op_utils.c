/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:09:46 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/15 17:16:02 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			insert(t_env *e, int pc, void *ptr, int size)
{
	unsigned char 	*tmp;

	tmp = (unsigned char*)ptr;
	if (pc < 0)
		pc = pc % MEM_SIZE + MEM_SIZE;
	while (size--)
		e->mem[(pc + size) % MEM_SIZE] = *tmp++;
}

int				param_sum(t_env *e, int pc, int size)
{
	int		res;
	int		j;

	res = e->mem[pc % MEM_SIZE];
	j = 1;
	while (--size)
	{
		res *= 256;
		res += e->mem[(pc + j++) % MEM_SIZE];
	}
	return ((size == 2) ? (short)res : res);
}

int				param_value(t_env *e, t_ocp check, int to_find, t_proc *ptr)
{
	int		value;

	value = 0;
	if (to_find == 1)
	{
		if (check.s1 == 1)
			value = ptr->r[e->mem[(ptr->pc + 2) % MEM_SIZE]];
		else if (check.s1 == 2)
			value = e->mem[param_sum(e, ptr->pc + 2, check.s1) % MEM_SIZE];
		else if (check.s1 == 4)
			value = param_sum(e, ptr->pc + 2, check.s1);
	}
	else if (to_find == 2)
	{
		if (check.s2 == 1)
			value = ptr->r[e->mem[(ptr->pc + 2 + check.s1) % MEM_SIZE]];
		else if (check.s2 == 2)
			value = e->mem[param_sum(e, ptr->pc + 2 + check.s1, check.s2) % MEM_SIZE];
		else if (check.s2 == 4)
			value = param_sum(e, ptr->pc + 2 + check.s1, check.s2);
	}
	return (value);
}

static t_ocp	find_param_size(t_ocp check, int ocp, int on_two)
{
	check.s1 = 0;
	check.s2 = 0;
	check.s3 = 0;
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

t_ocp			check_ocp(int ocp, int on_two)
{
	t_ocp	check;

	check.error = 0;
	check.p1 = 0;
	check.p2 = 0;
	check.p3 = 0;
	if (ocp < 64)
		check.error = 1;
	if (ocp >= 192 || ocp < 64)
		check.p1 = (ocp >= 192) ? 192 : 0;
	else
		check.p1 = (ocp >= 128) ? 128 : 64;
	if (ocp - check.p1 >= 48 || ocp - check.p1 < 16)
		check.p2 = (ocp - check.p1 >= 48) ? 48 : 0;
	else
		check.p2 = (ocp - check.p1 >= 32) ? 32 : 16;
	check.p3 = ocp - check.p1 - check.p2;
	return (find_param_size(check, ocp, on_two));
}
