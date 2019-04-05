/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 17:01:21 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void				init(t_env *e)
{
	e->c = 0;
	e->c_to_die = CYCLE_TO_DIE;
	e->c_total = 0;
	e->nb_champ = 0;
	ft_bzero(e->mem, MEM_SIZE);
}

void				place_champ(t_env *e)
{
	int part;
	int champ;
	int i;
	int j;

	champ = 0;
	part = MEM_SIZE / e->nb_champ;
	while (champ < e->nb_champ)
	{
		j = 0;
		i = champ * part;
		while (j < CHAMP_MAX_SIZE)
		{
			e->mem[i] = e->champs[champ].content[j];
			++i;
			++j;
		}
		champ++;
	}
}

int					main(int ac, char **av)
{
	t_env			e;
	int				i;

	init(&e);
	i = -1;
	if (!(parse_args(&e, ac, av)))
		return (1);
	while (++i < e.nb_champ)
	{
		if (!(check_champ(&e, av[e.champs[i].file], i)))
		{
			ft_printf("{#ff3333}PARSE KO{reset}\n\n");
			return (1);
		}
		else
			ft_printf("{green}PARSE OK{reset}\n\n");
	}
	place_champ(&e);
	play(&e);
	return (0);
}
