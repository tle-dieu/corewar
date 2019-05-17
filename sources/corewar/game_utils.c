/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 11:05:42 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/17 15:41:59 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <stdlib.h>

void			print_verbosity(t_env *e, t_proc *ptr)
{
	if (e->verbose >= 3 && !ptr->dead)
	{
		if (ptr->cycle && e->verbose == 4)
			ft_printf("Le processus %d du joueur %d doit attendre %d %s\n",
				ptr->id, ptr->owner, ptr->cycle,
				ptr->cycle <= 1 ? "cycle" : "cycles");
		else if (!ptr->cycle && ptr->op > 0 && ptr->op < 17)
			ft_printf("Le processus %d du joueur %d effectue l'op %d\n",
				ptr->id, ptr->owner, ptr->op);
	}
}

int				create_new_process(t_env *e, int pc, t_proc *ptr)
{
	int		i;
	t_proc	*new;

	i = -1;
	e->nb_proc++;
	if (!(new = (t_proc*)malloc(sizeof(t_proc))))
		freedom(e, 1);
	new->owner = ptr->owner;
	new->dead = 0;
	new->color = ptr->color;
	new->id = e->nb_proc;
	new->live = ptr->live;
	while (++i <= 17)
		new->r[i] = ptr->r[i];
	new->pc = pc;
	new->carry = ptr->carry;
	pc = pc % MEM_SIZE + (pc < 0 ? MEM_SIZE : 0);
	new->op = e->mem[pc % MEM_SIZE];
	new->cycle = choose_cycle(new->op);
	new->next = e->new_proc;
	if (!e->new_proc)
		e->proc_to_load = new;
	e->new_proc = new;
	return (1);
}

int				check_reg(int reg)
{
	return (reg > 0 && reg < 17);
}
