/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 21:26:03 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/01 20:05:58 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void				get_keys(t_env *e)
{
	int		key;

	timeout(0);
	if ((key = getch()) == ' ' || key == 43 || key == 45)
	{
		if (key == 43 && e->v.sleep_value)
			e->v.sleep_value -= 1000;
		else if (key == 45 && e->v.sleep_value < 150000)
			e->v.sleep_value += 1000;
		else if (key == ' ')
		{
			while (42)
				if (getch() == ' ')
					break ;
		}
	}
}

static void			init_color_map(t_env *e)
{
	int		i;
	int		j;
	int		champ;

	champ = -1;
	i = -1;
	while (++i < MEM_SIZE)
		e->v.color_map[i] = 6;
	while (++champ < e->nb_champ)
	{
		j = -1;
		i = champ * (MEM_SIZE / e->nb_champ);
		while (++j < e->champs[champ].size)
			e->v.color_map[i++] = champ + 2;
	}
	e->v.sleep_value = 50000;
}

void				visu(t_env *e)
{
	int		i;

	i = -1;
	initscr();
	curs_set(0);
	noecho();
	start_color();
	while (++i <= 4)
		e->v.posess[i] = 0;
	e->v.live_color = 0;
	init_color_map(e);
	init_pair(PLAYER_1, 32, COLOR_BLACK);
	init_pair(PLAYER_2, 28, COLOR_BLACK);
	init_pair(PLAYER_3, 1, COLOR_BLACK);
	init_pair(PLAYER_4, 11, COLOR_BLACK);
	init_pair(NO_ONE, 242, COLOR_BLACK);
	init_pair(WRITING, 253, COLOR_BLACK);
	init_pair(PROGRESS_1, 32, 32);
	init_pair(PROGRESS_2, 28, 28);
	init_pair(PROGRESS_3, 1, 1);
	init_pair(PROGRESS_4, 11, 11);
	init_pair(PROGRESS_NO, 242, 242);
	print_current_map(e);
	while (getch() != ' ')
		;
}
