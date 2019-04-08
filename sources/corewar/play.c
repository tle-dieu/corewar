/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/08 21:04:11 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int		choose_cycle(int op)
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
	return (-1);
}

static int		init_proc(t_env *e, int j, int begin)
{
	int		i;
	t_proc	*new;
	t_proc	*ptr;

	i = 0;
	ptr = e->champs[j].proc;
	if (!(new = (t_proc*)ft_memalloc(sizeof(t_proc))))
		return (0);
	if (!ptr)
	{
		new->id = 1;
		new->r[1] = e->champs[j].id;
	}
	else
		new->id = ptr->id + 1;
	new->pc = begin;
	new->owner = e->champs[j].id;
	new->op = e->champs[j].content[0];
	new->cycle = choose_cycle(new->op);
	new->next = ptr;
	e->champs[j].proc = new;
	return (1);
}

static void		place_champ(t_env *e)
{
	int		champ;
	int		i;
	int		j;

	champ = -1;
	while (++champ < e->nb_champ)
	{
		j = 0;
		i = champ * (MEM_SIZE / e->nb_champ);
		if (!(init_proc(e, champ, i)))
			ft_printf("malloc error - to free\n");
		while (j < CHAMP_MAX_SIZE)
			e->mem[i++] = e->champs[champ].content[j++];
	}
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
			if (ptr->cycle)
				ft_printf("Player %-25s :: remaining cycles %d, waiting for op %d\n",
					e->champs[i].name, ptr->cycle, ptr->op);
			if (!ptr->cycle)
			{
				ft_printf("PLAYER %s ===> OP %d\n", e->champs[i].name, ptr->op);
				if (ptr->pc + 12 > MEM_SIZE)
					ptr->pc = ptr->pc % MEM_SIZE;
				(*ft_ptr[e->mem[ptr->pc] - 1])(e, &ptr->pc, ptr);
				ptr->op = e->mem[ptr->pc];
				ptr->cycle = choose_cycle(e->mem[ptr->pc]);
				ft_printf("\n\n");
			}
			else
				ptr->cycle--;
			ptr = ptr->next;
		}
	}
	return (0);
}

void			play(t_env *e)
{
	int		check_nb;
	int		nb_live;

	place_champ(e);
	e->cycle = 0;
	check_nb = 0;
	e->living = e->nb_champ;
	while (1)
	{
		nb_live = exec_cycle(e);
		if (nb_live <= NBR_LIVE)
		{
			e->c_to_die -= CYCLE_DELTA;
			check_nb = 0;
		}
		else
		{
			++check_nb;
			if (!(check_nb % MAX_CHECKS))
				e->c_to_die -= CYCLE_DELTA;
		}
		e->cycle++;
		if (e->cycle == 30)
			break ;
		ft_printf("\n");
	}
	print_env(*e);
}
