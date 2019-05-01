/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/01 19:09:01 by acompagn         ###   ########.fr       */
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
	if (e.visu && e.dump == -1)
		visu(&e);
	play(&e);
	if (e.c_total == e.dump)
		print_memory(&e, 0);
	else if (e.c_total < e.dump)
		ft_printf("Incorrect dump %d: game ended at cycle %d\n",
			e.dump, e.c_total);
	if (e.visu && e.dump == -1)
		print_winner(&e);
	else if (e.dump == -1)
		print_subject_winner(&e);
	freedom(&e, 0);
	return (0);
}
