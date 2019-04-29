/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_1_5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:25:38 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/29 16:12:57 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		live(t_env *e, int *pc, t_proc *ptr)
{
	int		player_nb;
	int		j;

	*pc += 1;
	j = -1;
	e->nb_live++;
	ptr->live = 1;
	player_nb = param_sum(e, *pc, 4);
	while (++j < e->nb_champ)
		if (player_nb == e->champs[j].id)
		{
			if (e->visu)
				e->v.live_color = j + 2;
//			else
//				ft_printf("un processus dit que le joueur %d(%s) est en vie\n",
//					e->champs[j].id, e->champs[j].name);
			e->last_live = e->champs[j].id;
			e->champs[j].nb_live++;
		}
	*pc += 4;
}

void		ld(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		addr;
	int		reg1;
	int		reg2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	reg1 = e->mem[(*pc + 2 + check.s1) % MEM_SIZE];
	reg2 = e->mem[(*pc + 2) % MEM_SIZE];
	if (!check.error && reg1 > 0 && reg1 < 17)
	{
		if (check.s1 == 1 && reg2 > 0 && reg2 < 17)
			ptr->r[reg1] = ptr->r[reg2];
		else if (check.s1 == 2)
		{
			addr = param_sum(e, *pc + 3, 1);
			ptr->r[reg1] = param_sum(e, *pc + (addr % IDX_MOD), REG_SIZE);
		}
		else if (check.s1 == 4)
			ptr->r[reg1] = param_value(e, check, 1, ptr);
		ptr->carry = !ptr->r[reg1];
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		st(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	short	addr;
	int		reg1;
	int		reg2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	reg1 = e->mem[(*pc + 2) % MEM_SIZE];
	reg2 = e->mem[(*pc + 2 + check.s1) % MEM_SIZE];
	if (!check.error)
	{
		if (check.s2 == 1 && reg1 > 0 && reg2 > 0 && reg1 < 17 && reg2 < 17)
			ptr->r[reg2] = ptr->r[reg1];
		else if (check.s2 == 2 && check_reg(e->mem[(*pc + 2) % MEM_SIZE]))
		{
			addr = param_sum(e, *pc + 3, 2);
			e->v.color = e->visu ? ptr->color : 0;
			insert(e, (*pc + (addr % IDX_MOD)) % MEM_SIZE,
				(void*)&ptr->r[reg1], REG_SIZE);
		}
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}

void		add(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check_reg(e->mem[(*pc + 2) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 3) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 4) % MEM_SIZE]))
		check.error = 1;
	if (!check.error)
	{
		v1 = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
		v2 = ptr->r[e->mem[(*pc + 3) % MEM_SIZE]];
		ptr->r[e->mem[(*pc + 4) % MEM_SIZE]] = v1 + v2;
		ptr->carry = (!ptr->r[e->mem[(*pc + 4) % MEM_SIZE]]);
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3 + check.s3;
}

void		sub(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		v1;
	int		v2;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
		g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check_reg(e->mem[(*pc + 2) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 3) % MEM_SIZE])
		|| !check_reg(e->mem[(*pc + 4) % MEM_SIZE]))
		check.error = 1;
	if (!check.error)
	{
		v1 = ptr->r[e->mem[(*pc + 2) % MEM_SIZE]];
		v2 = ptr->r[e->mem[(*pc + 3) % MEM_SIZE]];
		ptr->r[e->mem[(*pc + 4) % MEM_SIZE]] = v1 - v2;
		ptr->carry = (!ptr->r[e->mem[(*pc + 4) % MEM_SIZE]]);
	}
	*pc = *pc + 2 + check.s1 + check.s2 + check.s3;
}
