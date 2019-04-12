/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/12 03:27:21 by tle-dieu         ###   ########.fr       */
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

static int		exec_cycle(t_env *e)
{
	int		i;
	t_proc	*ptr;
	void	(*ft_ptr[16])() = {live, ld, st, add, sub, and, or, xor,
		zjmp, ldi, sti, op_fork, lld, lldi, lfork, aff};

	i = e->nb_champ;
	while (i--)
	{
		ptr = e->champs[i].proc;
		while (ptr)
		{
			if (!ptr->cycle)
			{
				if (PRINT)
				{
					if (i == 0)
						ft_printf("{#0bd185}%-25s{reset} ===> OP %d\n", e->champs[i].name, ptr->op);
					else if (i == 1)
						ft_printf("{#f4c302}%-25s{reset} ===> OP %d\n", e->champs[i].name, ptr->op);
				}
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
	t_proc	*ptr;

	i = -1;
	while (++i < e->nb_champ)
	{
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
	int		nb_check;
	int		i;

	place_champ(e);
	nb_check = 0;
	while (e->c_to_die > 0)
	{
		exec_cycle(e);
		if (e->cycle == e->c_to_die)
		{
			if (e->nb_live >= NBR_LIVE)
				e->c_to_die -= CYCLE_DELTA;
			is_alive(e);
			e->c_to_die -= CYCLE_DELTA;
			if (nb_check && !(nb_check % MAX_CHECKS))
				e->c_to_die -= CYCLE_DELTA;
			e->cycle = 0;
			e->nb_live = 0;
			++nb_check;
		}
		e->cycle++;
	}
	i = -1;
	ft_printf("\n\n");
	while (++i < e->nb_champ)
	{
		if (e->last_live == e->champs[i].id)
			ft_printf("WINNER {#0bd185}%s(%d){reset}\n", e->champs[i].name, e->champs[i].id);
		else
			ft_printf("LOOSER {#f48042}%s(%d){reset}\n", e->champs[i].name, e->champs[i].id);
	}
	ft_printf("\n\n");
}
