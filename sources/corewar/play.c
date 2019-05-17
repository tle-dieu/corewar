/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/17 15:09:53 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int					choose_cycle(int op)
{
	if (op > 0 && op < 17)
		return (g_op_tab[op - 1].nb_cycle);
	else
		return (1);
}

static int			exec_cycle(t_env *e, t_proc *ptr)
{
	!e->visu && e->verbose >= 2 ? ft_printf("> Cycle %d <\n", e->c_total) : 1;
	while (ptr)
	{
		ptr->cycle--;
		!ptr->dead && e->verbose && !e->visu ? print_verbosity(e, ptr) : 1;
		ptr->pc = ptr->pc % MEM_SIZE + (ptr->pc < 0 ? MEM_SIZE : 0);
		if (!ptr->dead && e->visu && e->dump == -1)
			e->v.map[ptr->pc % MEM_SIZE] = -e->v.map[ptr->pc % MEM_SIZE];
		if (!ptr->dead && (ptr->op < 1 || ptr->op > 16))
		{
			ptr->pc++;
			ptr->op = e->mem[ptr->pc % MEM_SIZE];
			ptr->cycle = choose_cycle(e->mem[ptr->pc % MEM_SIZE]);
		}
		else if (!ptr->dead && !ptr->cycle)
		{
			g_op_tab[ptr->op - 1].ft_ptr(e, &ptr->pc, ptr);
			ptr->op = e->mem[ptr->pc % MEM_SIZE];
			ptr->pc = ptr->pc % MEM_SIZE + (ptr->pc < 0 ? MEM_SIZE : 0);
			ptr->cycle = choose_cycle(ptr->op);
		}
		ptr = ptr->next;
	}
	return (0);
}

static void			is_alive(t_env *e, t_proc *ptr)
{
	int		i;

	i = -1;
	if (e->verbose >= 2 && !e->visu)
		ft_printf("cycle_to_die: %d\n", e->c_to_die);
	if (e->visu && e->dump == -1)
		while (++i < e->nb_champ)
			e->champs[i].nb_live = 0;
	while (ptr)
	{
		if (!ptr->dead)
		{
			if (ptr->live)
				ptr->live = 0;
			else
			{
				e->nb_proc--;
				ptr->dead = 1;
			}
		}
		ptr = ptr->next;
	}
}

static void			add_new_proc(t_env *e)
{
	e->proc_to_load->next = e->proc;
	e->proc = e->new_proc;
	e->new_proc = NULL;
}

void				play(t_env *e)
{
	while (e->c_to_die > 0)
	{
		if (e->c_total == e->dump)
			break ;
		++e->c_total;
		++e->cycle;
		exec_cycle(e, e->proc);
		e->visu && e->dump == -1 ? print_current_map(e) : 1;
		e->new_proc ? add_new_proc(e) : 1;
		if (e->cycle == e->c_to_die)
		{
			e->visu ? ft_printf("\a") : 1;
			if (!e->nb_live)
				return ;
			if (e->nb_live < NBR_LIVE)
				++e->nb_check;
			if (e->nb_live >= NBR_LIVE
				|| (e->nb_check && !(e->nb_check % MAX_CHECKS)))
				e->c_to_die -= CYCLE_DELTA;
			is_alive(e, e->proc);
			e->total_live += e->nb_live;
			e->cycle = 0;
			e->nb_live = 0;
		}
	}
}
