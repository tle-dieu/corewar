/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:09:46 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/01 12:25:34 by acompagn         ###   ########.fr       */
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
	int		tmp;

	res = e->mem[pc % MEM_SIZE];
	j = 1;
	tmp = size;
	while (--size)
	{
		res *= 256;
		res += e->mem[(pc + j++) % MEM_SIZE];
	}
	return ((tmp == 2) ? (short)res : res);
}

int				param_value_1(t_env *e, t_ocp ch, t_proc *ptr, int mod)
{
	int		p;
	int		index;

	p = param_sum(e, (ptr->pc + 2) % MEM_SIZE, ch.s1);
	if (ch.s1 == 1)
	{
		ch.error = p < 1 || p > 16 ? 1 : ch.error;
		return (p < 1 || p > 16 ? 0 : ptr->r[p]);
	}
	else if (ch.s1 == 2 && ch.p1 == 192)
	{
		index = (ptr->pc + mod ? (p % IDX_MOD) : p) % MEM_SIZE;
		index = index < 0 ? index % MEM_SIZE + MEM_SIZE : index;
		return (e->mem[index]);
	}
	else if (ch.s1 == 4 || (ch.s1 == 2 && ch.p1 == 128))
		return (p);
	return (0);
}

int				param_value_2(t_env *e, t_ocp ch, t_proc *ptr, int mod)
{
	int		p;
	int		index;

	p = param_sum(e, (ptr->pc + 2 + ch.s1) % MEM_SIZE, ch.s2);
	if (ch.s2 == 1)
	{
		ch.error = p < 1 || p > 16 ? 1 : ch.error;
		return (p < 1 || p > 16 ? 0 : ptr->r[p]);
	}
	else if (ch.s2 == 2 && ch.p2 == 48)
	{
		index = (ptr->pc + mod ? (p % IDX_MOD) : p) % MEM_SIZE;
		index = index < 0 ? index % MEM_SIZE + MEM_SIZE : index;
		return (e->mem[index]);
	}
	else if (ch.s2 == 4 || (ch.s2 == 2 && ch.p2 == 32))
		return (p);
	return (1);
}

int				param_value_3(t_env *e, t_ocp ch, t_proc *ptr, int mod)
{
	int		p;
	int		index;

	p = param_sum(e, (ptr->pc + 2 + ch.s1 + ch.s2) % MEM_SIZE, ch.s3);
	if (ch.s3 == 1)
	{
		ch.error = p < 1 || p > 16 ? 1 : ch.error;
		return (p < 1 || p > 16 ? 0 : ptr->r[p]);
	}
	else if (ch.s3 == 2 && ch.p3 == 12)
	{
		index = (ptr->pc + mod ? (p % IDX_MOD) : p) % MEM_SIZE;
		index = index < 0 ? index % MEM_SIZE + MEM_SIZE : index;
		return (e->mem[index]);
	}
	else if (ch.s3 == 4 || (ch.s2 == 2 && ch.p2 == 8))
		return (p);
	return (1);
}
