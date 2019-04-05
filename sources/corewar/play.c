/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 16:26:03 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 17:03:29 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int exec_cycle(t_env *e)
{
	(void)e;
	ft_printf("EXEC_CYCLE\n");
	return (0);
}

void play(t_env *e)
{
	int check_nb;
	int nb_live;
	
	ft_printf("{yellow}PLAY\n\n\n");
	e->c = 0;
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
	e->c++;
	print_env(*e);
}
