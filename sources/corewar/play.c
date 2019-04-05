/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 18:40:25 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int	init_proc(t_env *e, int champ, int pc)
{
	int i;

	i = 0;
	ft_printf("{yellow}INIT_PROC{reset}\n\n");
	if (!(e->champs[champ].proc = (t_proc*)malloc(sizeof(t_proc))))
		return (0);
	e->champs[champ].proc->id = 1;
	e->champs[champ].proc->live = 0;
	e->champs[champ].proc->r[0] = e->champs[champ].id;
	while (i < 16)
		e->champs[champ].proc->r[++i] = 0;
	e->champs[champ].proc->pc = pc;
	e->champs[champ].proc->carry = 0;
	e->champs[champ].proc->next = NULL;
	ft_printf("{yellow}END_INIT_PROC{reset}\n\n");
	return (1);
}

static void	place_champ(t_env *e)
{
	int		part;
	int		champ;
	int		i;
	int		j;

	champ = 0;
	part = MEM_SIZE / e->nb_champ;
	while (champ < e->nb_champ)
	{
		j = 0;
		i = champ * part;
		init_proc(e, champ, i);
		while (j < CHAMP_MAX_SIZE)
		{
			e->mem[i] = e->champs[champ].content[j];
			++i;
			++j;
		}
		champ++;
	}
}

static int	exec_cycle(t_env *e)
{
	(void)e;
	ft_printf("EXEC_CYCLE\n");
	return (0);
}

void		play(t_env *e)
{
	int		check_nb;
	int		nb_live;

	place_champ(e);
	e->cycle = 0;
	check_nb = 0;
	e->living = e->nb_champ;
	nb_live = exec_cycle(e);
	if (nb_live <= NBR_LIVE)
	{
		e->c_to_die -= CYCLE_DELTA;
		check_nb = 0;
	}
	else
	{
		++check_nb;
		if (check_nb % MAX_CHECKS == 0)
			e->c_to_die -= CYCLE_DELTA;
	}
	e->cycle++;
	print_env(*e);
}
