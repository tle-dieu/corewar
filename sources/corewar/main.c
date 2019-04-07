/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/07 19:41:35 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void			init(t_env *e)
{
	int		i;

	i = -1;
	while (++i < 4)
	{
		e->champs[i].id = 0;
		e->champs[i].chosen_id[0] = 0;
		e->champs[i].chosen_id[1] = 0;
	}
	e->cycle = 0;
	e->c_to_die = CYCLE_TO_DIE;
	e->c_total = 0;
	e->nb_champ = 0;
	ft_bzero(e->mem, MEM_SIZE);
}

static void			attribute_id(t_env *e)
{
	int		i;
	int		j;
	int		new_id;

	i = -1;
	new_id = -1;
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
	i = -1;
	while (++i < e->nb_champ)
		ft_printf("Champ %d has id %d\n", i, e->champs[i].id);
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
	//que fait-on si deux joueurs ont le meme choix d'id ? tant pis?;
	attribute_id(&e);
	play(&e);
	return (0);
}
