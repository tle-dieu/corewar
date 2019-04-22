/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 11:05:42 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/22 11:11:12 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int				create_new_process(t_env *e, int pc, t_proc *ptr)
{
	int		i;
	t_proc	*new;

	i = -1;
	e->nb_proc++;
	if (!(new = (t_proc *)ft_memalloc(sizeof(t_proc))))
		return (0);
	new->owner = ptr->owner;
	new->color = ptr->color;
	new->live = ptr->live;
	new->carry = ptr->carry;
	new->op = e->mem[pc];
	new->cycle = choose_cycle(new->op);
	while (++i <= 17)
		new->r[i] = ptr->r[i];
	new->pc = pc;
	new->id = e->nb_proc;
	new->next = e->new_proc;
	e->new_proc = new;
	return (1);
}
