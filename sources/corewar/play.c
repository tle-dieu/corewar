/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/26 12:15:43 by acompagn         ###   ########.fr       */
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

static int			exec_cycle(t_env *e)
{
	t_proc	*ptr;

	ptr = e->proc;
	while (ptr)
	{
		ptr->cycle--;
		if (e->visu && e->dump == -1)
			e->v.color_map[ptr->pc % MEM_SIZE] =
				-e->v.color_map[ptr->pc % MEM_SIZE];
		if (!ptr->cycle)
		{
			if (ptr->pc >= MEM_SIZE)
				ptr->pc = ptr->pc % MEM_SIZE;
			if (ptr->op < 1 || ptr->op > 16)
				ptr->pc++;
			else
				g_op_tab[ptr->op - 1].ft_ptr(e, &ptr->pc, ptr);
			ptr->op = e->mem[ptr->pc % MEM_SIZE];
			ptr->cycle = choose_cycle(e->mem[ptr->pc % MEM_SIZE]);
		}
		ptr = ptr->next;
	}
	return (0);
}

static void			is_alive(t_env *e)
{
	t_proc	*tmp;
	t_proc	*ptr;
	int		i;

	ptr = e->proc;
	i = -1;
	if (e->visu && e->dump == -1)
		while (++i < e->nb_champ)
			e->champs[i].nb_live = 0;
	while (ptr)
	{
		tmp = NULL;
		if (ptr->live)
			ptr->live = 0;
		else
			tmp = ptr;
		ptr = ptr->next;
		if (tmp)
			destroy_process(e, tmp);
	}
}

static void			add_new_proc(t_env *e)
{
	t_proc		*ptr;

	ptr = e->new_proc;
	while (ptr && ptr->next)
		ptr = ptr->next;
	ptr->next = e->proc;
	e->proc = e->new_proc;
	e->new_proc = NULL;
}

void				play(t_env *e)
{
	while (e->c_to_die > 0)
	{
		exec_cycle(e);
		e->visu && e->dump == -1 ? print_current_map(e) : 1;
		if (e->new_proc)
			add_new_proc(e);
		if (e->cycle == e->c_to_die)
		{
			ft_printf("\a");
			if (!e->nb_live)
				break ;
			if (e->nb_live < NBR_LIVE)
				++e->nb_check;
			if (e->nb_live >= NBR_LIVE
					|| (e->nb_check && !(e->nb_check % MAX_CHECKS)))
				e->c_to_die -= CYCLE_DELTA;
			is_alive(e);
			e->total_live += e->nb_live;
			e->cycle = 0;
			e->nb_live = 0;
		}
		(e->c_total == e->dump) ? print_memory(e, 0) : 1;
		e->c_total++;
		e->cycle++;
	}
}
