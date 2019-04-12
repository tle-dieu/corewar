/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:57:57 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/12 20:02:07 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		attribute_id(t_env *e)
{
	int		i;
	int		j;
	int		new_id;

	i = -1;
	new_id = -1;
	while (++i < e->nb_champ)
	{
		if (e->champs[i].chosen_id[0])
		{
			j = i;
			while (++j < e->nb_champ)
				if (e->champs[j].chosen_id[0] && e->champs[j].chosen_id[1] == e->champs[i].chosen_id[1])
					e->champs[j].chosen_id[1]--;
		}
	}
	i = -1;
	while (++i < e->nb_champ)
	{
		if (!(e->champs[i].chosen_id[0]))
		{
			j = -1;
			while (++j < e->nb_champ)
				if (i != j && e->champs[j].id == new_id)
					new_id--;
			e->champs[i].id = new_id;
		}
		else
			e->champs[i].id = e->champs[i].chosen_id[1];
	}
}

int			create_new_process(t_env *e, int pc, t_proc *ptr)
{
	int		i;
	t_proc	*new;
	t_proc	*tmp;

	i = -1;
	if (!(new = (t_proc *)ft_memalloc(sizeof(t_proc))))
		return (0);
	new->owner = ptr->owner;
	new->live = ptr->live;
	new->carry = ptr->carry;
	new->op = e->mem[pc];
	new->cycle = choose_cycle(new->op);
	while (++i < 16)
		new->r[i] = ptr->r[i];
	new->pc = pc;
	i = -1;
	while (++i < e->nb_champ)
	{
		if (e->champs[i].id == ptr->owner)
		{
			tmp = e->champs[i].proc;
			new->id = tmp->id + 1;
			new->next = e->champs[i].proc;
			e->champs[i].proc = new;
			break ;
		}
	}
	return (1);
}

void			init(t_env *e)
{
	int		i;

	i = -1;
	while (++i < 4)
	{
		e->champs[i].id = 0;
		e->champs[i].chosen_id[0] = 0;
		e->champs[i].chosen_id[1] = 0;
	}
	e->nb_check = 0;
	e->cycle = 0;
	e->dump = -1;
	e->nb_live = 0;
	e->total_live = 0;
	e->last_live = 0;
	e->c_to_die = CYCLE_TO_DIE;
	e->c_total = 0;
	e->nb_champ = 0;
	ft_bzero(e->mem, MEM_SIZE);
}

int				init_proc(t_env *e, int j, int begin)
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

void			place_champ(t_env *e)
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
			freedom(e);
		while (j < CHAMP_MAX_SIZE)
			e->mem[i++] = e->champs[champ].content[j++];
	}
}
