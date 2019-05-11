/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:09:46 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/11 16:52:04 by acompagn         ###   ########.fr       */
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
			e->v.map[(pc + size) % MEM_SIZE] = e->v.color;
	}
}

int				param_sum(t_env *e, int pc, int size)
{
	int		res;
	int		j;
	int		tmp;

	if (pc < 0)
		pc = pc % MEM_SIZE + MEM_SIZE;
	res = e->mem[pc % MEM_SIZE];
	j = 1;
	tmp = size;
	while (size--)
		res += e->mem[pc++ % MEM_SIZE] << 8 * size;
	return ((tmp == 2) ? (short)res : res);
}

void			param_value(t_env *e, t_ocp *ch, t_proc *ptr, int mod)
{
	int		p;
	int		i;
	int		index;
	int		size;

	i = -1;
	size = ptr->pc + 2;
	while (++i < g_op_tab[ptr->op - 1].nb_param)
	{
		p = param_sum(e, size % MEM_SIZE, ch->s[i]);
		if (ch->s[i] == 1)
		{
			ch->error = p < 1 || p > 16 ? 1 : ch->error;
			ch->v[i] = (p < 1 || p > 16 ? 0 : ptr->r[p]);
		}
		else if (ch->s[i] == 2 && ch->p[i] == ch->shift * 3)
		{
			index = (ptr->pc + mod ? (p % IDX_MOD) : p) % MEM_SIZE;
			ch->v[i] = e->mem[index < 0 ? index % MEM_SIZE + MEM_SIZE : index];
		}
		else if (ch->s[i] == 4 || (ch->s[i] == 2 && ch->p[i] == ch->shift * 2))
			ch->v[i] = p;
		size += ch->s[i];
		ch->shift = ch->shift >> 2;
	}
}
