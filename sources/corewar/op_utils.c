/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:09:46 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/29 11:02:02 by acompagn         ###   ########.fr       */
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
