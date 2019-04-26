/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:09:46 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/24 19:26:50 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			insert(t_env *e, int pc, void *ptr, int size)
{
	unsigned char	*tmp;

	tmp = (unsigned char*)ptr;
	if (pc < 0)
		pc = pc % MEM_SIZE + MEM_SIZE;
	while (size--)
	{
		e->mem[(pc + size) % MEM_SIZE] = *tmp++;
		if (e->visu)
			e->v.color_map[(pc + size) % MEM_SIZE] = e->v.color;
	}
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

int				param_value(t_env *e, t_ocp ch, int to_find, t_proc *ptr)
{
	int		value;

	value = 0;
	if (to_find == 1)
	{
		if (ch.s1 == 1)
			value = ptr->r[e->mem[(ptr->pc + 2) % MEM_SIZE]];
		else if (ch.s1 == 2 && ch.p1 == 192)
			value = e->mem[param_sum(e, ptr->pc + 2, ch.s1) % MEM_SIZE];
		else if (ch.s1 == 4 || (ch.s1 == 2 && ch.p1 == 128))
			value = param_sum(e, ptr->pc + 2, ch.s1);
	}
	else if (to_find == 2)
	{
		if (ch.s2 == 1)
			value = ptr->r[e->mem[(ptr->pc + 2 + ch.s1) % MEM_SIZE]];
		else if (ch.s2 == 2 && ch.p2 == 48)
			value = e->mem[param_sum(e, ptr->pc + 2 + ch.s1, ch.s2) % MEM_SIZE];
		else if (ch.s2 == 4 || (ch.s2 == 2 && ch.p2 == 32))
			value = param_sum(e, ptr->pc + 2 + ch.s1, ch.s2);
	}
	return (value);
}

static t_ocp	find_param_size(t_ocp check, int ocp, int on_two)
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

t_ocp			check_ocp(int ocp, int on_two)
{
	t_ocp	check;

	check = (t_ocp){0, 0, 0, 0, 0, 0, 0};
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
