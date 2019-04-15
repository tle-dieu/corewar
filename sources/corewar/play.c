/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/15 18:14:54 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int				choose_cycle(int op)
{
	if (op > 0 && op < 17)
		return (g_op_tab[op - 1].nb_cycle);
	else
		return (1);
}

static void		init_ft_ptr(void (*ft_ptr[])(t_env *e, int *pc, t_proc *ptr))
{
	ft_ptr[0] = live;
	ft_ptr[1] = ld;
	ft_ptr[2] = st;
	ft_ptr[3] = add;
	ft_ptr[4] = sub;
	ft_ptr[5] = and;
	ft_ptr[6] = or;
	ft_ptr[7] = xor;
	ft_ptr[8] = zjmp;
	ft_ptr[9] = ldi;
	ft_ptr[10] = sti;
	ft_ptr[11] = op_fork;
	ft_ptr[12] = lld;
	ft_ptr[13] = lldi;
	ft_ptr[14] = lfork;
	ft_ptr[15] = aff;
}

static void		print_game(t_env *e, t_proc *ptr)
{
	if (ptr->owner == -1)
		ft_printf("{#0bd185}%-25d{reset} ===> OP %d PROCESS %d \n",
			ptr->owner, ptr->op, ptr->id);
	else
		ft_printf("{#f4428c}%-25d{reset} ===> OP %d PROCESS %d  | CYCLE %d\n",
			ptr->owner, ptr->op, ptr->id, e->c_total);
}

static int		exec_cycle(t_env *e)
{
	t_proc	*ptr;
	void	(*ft_ptr[16])();
	int		tmp;

	init_ft_ptr(ft_ptr);
	ptr = e->proc;
	while (ptr)
	{
		ptr->cycle--;
		if (!ptr->cycle)
		{
			PRINT && ptr->op ? print_game(e, ptr) : 1;
			tmp = ptr->pc;
			if (ptr->pc >= MEM_SIZE)
				ptr->pc = ptr->pc % MEM_SIZE;
			if (ptr->op < 1 || ptr->op > 16 || e->mem[ptr->pc % MEM_SIZE] < 1
					|| e->mem[ptr->pc % MEM_SIZE] > 17)
				ptr->pc++;
			else
				(*ft_ptr[ptr->op - 1])(e, &ptr->pc, ptr);
			ptr->op = e->mem[ptr->pc % MEM_SIZE];
			ptr->cycle = choose_cycle(e->mem[ptr->pc % MEM_SIZE]);
		}
		ptr = ptr->next;
	}
	return (0);
}

static void		is_alive(t_env *e)
{
	t_proc	*tmp;
	t_proc	*ptr;

	ptr = e->proc;
	while (ptr)
	{
		tmp = NULL;
		if (ptr->live)
			ptr->live = 0;
		else if (!ptr->live || ptr->cycle)
			tmp = ptr;
		ptr = ptr->next;
		if (tmp)
			destroy_process(e, tmp);
	}
}

void			add_new_proc(t_env *e)
{
	t_proc		*ptr;

	ptr = e->new_proc;
	while (ptr && ptr->next)
		ptr = ptr->next;
	ptr->next = e->proc;
	e->proc = e->new_proc;
	e->new_proc = NULL;
}

void			play(t_env *e)
{
	while (e->c_to_die > 0)
	{
		exec_cycle(e);
		if (e->new_proc)
			add_new_proc(e);
		if (e->cycle == e->c_to_die)
		{
			if (e->nb_live < NBR_LIVE)
				++e->nb_check;
			if (e->nb_live >= NBR_LIVE || (e->nb_check && !(e->nb_check % MAX_CHECKS)))
				e->c_to_die -= CYCLE_DELTA;
			is_alive(e);
			e->total_live += e->nb_live;
			e->cycle = 0;
			e->nb_live = 0;
		}
		if (e->dump != -1 && e->c_total == e->dump)
		{
			print_memory(e);
			break ;
		}
		e->c_total++;
		e->cycle++;
	}
	ft_printf("\n\ne->dump = %d\ne->c_total = %d\nCTD %d\n", e->dump, e->c_total, e->c_to_die);
}
