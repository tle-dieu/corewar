/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/12 13:34:06 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int				choose_cycle(int op)
{
	if (op == 16)
		return (2);
	else if (op == 2 || op == 3)
		return (5);
	else if (op == 6 || op == 7 || op == 8)
		return (6);
	else if (op == 1 || op == 4 || op == 5 || op == 13)
		return (10);
	else if (op == 9)
		return (20);
	else if (op == 10 || op == 11)
		return (25);
	else if (op == 14)
		return (50);
	else if (op == 12)
		return (800);
	else if (op == 15)
		return (1000);
	return (0);
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

static int		exec_cycle(t_env *e)
{
	int		i;
	t_proc	*ptr;
	void	(*ft_ptr[16])();

	init_ft_ptr(ft_ptr);
	i = e->nb_champ;
	while (i--)
	{
		ptr = e->champs[i].proc;
		while (ptr)
		{
			if (!ptr->cycle)
			{
				(PRINT && !i) ? ft_printf("{#0bd185}%-25s{reset} ===> OP %d\n",
						e->champs[i].name, ptr->op) : 1;
				(PRINT && i == 1) ? ft_printf("{#f4c302}%-25s{reset} ===> OP %d\n",
						e->champs[i].name, ptr->op) : 1;
				if (ptr->pc + 12 > MEM_SIZE)
					ptr->pc = ptr->pc % MEM_SIZE;
				if (ptr->op < 1 || ptr->op > 16)
					ptr->pc++;
				else
					(*ft_ptr[e->mem[ptr->pc] - 1])(e, &ptr->pc, ptr);
				ptr->op = e->mem[ptr->pc % MEM_SIZE];
				ptr->cycle = choose_cycle(e->mem[ptr->pc % MEM_SIZE]);
			}
			else
				ptr->cycle--;
			ptr = ptr->next;
		}
	}
	return (0);
}

static void		is_alive(t_env *e)
{
	int		i;
	int		tmp;
	t_proc	*ptr;

	i = -1;
	while (++i < e->nb_champ)
	{
		tmp = 0;
		ptr = e->champs[i].proc;
		while (ptr)
		{
			if (ptr->live)
				ptr->live = 0;
			else
				destroy_process(e, i, ptr);
			ptr = ptr->next;
		}
	}
}

void			play(t_env *e)
{
	while (e->c_to_die > 0)
	{
		exec_cycle(e);
		if (e->cycle == e->c_to_die)
		{
			if (e->nb_live >= NBR_LIVE)
				e->c_to_die -= CYCLE_DELTA;
			is_alive(e);
			e->c_to_die -= CYCLE_DELTA;
			if (e->nb_check && !(e->nb_check % MAX_CHECKS))
				e->c_to_die -= CYCLE_DELTA;
			e->cycle = 0;
			e->total_live += e->nb_live;
			e->nb_live = 0;
			++e->nb_check;
		}
		if (e->dump != -1 && e->c_total == e->dump)
		{
			print_memory(e);
			break ;
		}
		e->c_total++;
		e->cycle++;
	}
}
