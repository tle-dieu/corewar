/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/18 18:42:26 by acompagn         ###   ########.fr       */
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
	t_ocp	check;

	ptr = e->proc;
	while (ptr)
	{
		ptr->cycle--;
		if (VISU)
			e->color_map[ptr->pc % MEM_SIZE] = -e->color_map[ptr->pc % MEM_SIZE];
		if (!ptr->cycle)
		{
			PRINT && ptr->op ? print_game(e, ptr) : 1;
			if (ptr->pc >= MEM_SIZE)
				ptr->pc = ptr->pc % MEM_SIZE;
			check = check_ocp(e->mem[ptr->pc + 1],
					g_op_tab[ptr->op - 1].dir_size);
			if (ptr->op < 1 || ptr->op > 16 || e->mem[ptr->pc % MEM_SIZE] < 1
					|| e->mem[ptr->pc % MEM_SIZE] > 17)
				ptr->pc++;
			else if (check.error == -1)
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

	ptr = e->proc;
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
		VISU ? print_current_map(e) : 1;
		if (e->new_proc)
			add_new_proc(e);
		if (e->cycle == e->c_to_die)
		{
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
		if (e->dump != -1 && e->c_total == e->dump)
		{
			print_memory(e, 0);
			break ;
		}
		e->c_total++;
		e->cycle++;
	}
}
