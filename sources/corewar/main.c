/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/17 12:52:47 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int					main(int ac, char **av)
{
	t_env			e;
	int				i;

	init(&e);
	i = -1;
	if (!(parse_args(&e, ac, av)))
		return (1);
	while (++i < e.nb_champ)
		if (!(check_champ(&e, av[e.champs[i].file], i)))
			return (1);
	attribute_id(&e);
	place_champ(&e);
	e.visu && e.dump == -1 ? visu(&e) : 1;
	play(&e);
	e.c_total == e.dump ? print_memory(&e, 0) : 1;
	if (e.c_total < e.dump)
		ft_dprintf(2, "{bold}Incorrect dump %d: game ended at cycle %d{R}\n",
			e.dump, e.c_total);
	if (e.dump == -1)
		e.visu ? print_winner(&e) : print_subject_winner(&e);
	freedom(&e, 0);
	return (0);
}
