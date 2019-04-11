/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/11 17:06:11 by tle-dieu         ###   ########.fr       */
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
	t_proc	*new;
	t_proc	*ptr;

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
			if (!ptr->cycle)
			{
				if (i == 0)
					ft_printf("{#f40202}%s{reset} ===> OP %d\n", e->champs[i].name, ptr->op);
				else if (i == 1)
					ft_printf("{#f4c302}%s{reset} ===> OP %d\n", e->champs[i].name, ptr->op);
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

void			print_register(t_env *e)
{
	int		i;
	int		j;
	t_proc	*ptr;

	i = e->nb_champ;
	while (i--)
	{
		ptr = e->champs[i].proc;
		ft_printf("\nREGISTERS of champ: %s\n", e->champs[i].name);
		while (ptr)
		{
			j = -1;
			while (++j < 17)
				ft_printf("r[%d] = %d ", j, ptr->r[j]);
			ptr = ptr->next;
			ft_printf("\n");
		}
	}
}

void			destroy_process(t_env *e, int i, t_proc *to_del)
{
	t_proc	*ptr;
	t_proc	*tmp;

	ft_printf("\n\n>> Killing one process of player %s <<\n\n", e->champs[i].name);
	ptr = e->champs[i].proc;
	if (ptr == to_del)
	{
		tmp = ptr;
		e->champs[i].proc = ptr->next;
		free(tmp);
		return ;
	}
	while (ptr->next)
	{
		ft_printf("B\n");
		if (ptr->next == to_del)
		{
			tmp = ptr->next;
			ptr->next = ptr->next->next;
			free(tmp);
		}
		ptr = ptr->next;
	}
}

void			destroy_all(t_env *e, int i)
{
	t_proc	*ptr;
	t_proc	*tmp;

	ft_printf("\n\n>> Killing all processes of player %s <<\n\n", e->champs[i].name);
	ptr = e->champs[i].proc;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	e->champs[i].proc = NULL;
}

void			is_alive(t_env *e, int *living)
{
	int		i;
	int		tmp;
	t_proc	*ptr;

	i = -1;
	*living = 0;
	while (++i < e->nb_champ)
	{
		tmp = 0;
		ptr = e->champs[i].proc;
		while (ptr)
		{
			if (ptr->live && !ptr->cycle)
			{
				tmp++;
				ptr->live = 0;
			}
			else
				destroy_process(e, i, ptr);
			ptr = ptr->next;
		}
		if (tmp)
			*living = *living + 1;
		e->champs[i].alive = 0;
	}
}

void			play(t_env *e)
{
	int		nb_check;
	int		living;
	int		i;

	place_champ(e);
	e->cycle = 0;
	e->last_live = 0;
	nb_check = 0;
	living = e->nb_champ;
	while (e->c_to_die > 0)
	{
		exec_cycle(e);
		if (e->cycle == e->c_to_die)
		{
			if (e->nb_live >= NBR_LIVE)
				e->c_to_die -= CYCLE_DELTA;
			is_alive(e, &living);
			e->c_to_die -= CYCLE_DELTA;
			e->cycle = 0;
			e->nb_live = 0;
			++nb_check;
		}
		if (nb_check && !(nb_check % MAX_CHECKS))
			e->c_to_die -= CYCLE_DELTA;
		e->cycle++;
	}
   	i = -1;
	while (++i < e->nb_champ)
		ft_printf("Player %d == %s\n", e->champs[i].id, e->champs[i].name);
//		print_chmp(e, i, 0);
//	print_register(e);
	ft_printf("\nCTD = %d\n\n\nLAST LIVE from player %d\n", e->c_to_die, e->last_live);
}
