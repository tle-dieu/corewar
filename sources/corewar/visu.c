/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 21:26:03 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/19 16:07:55 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void				init_color_map(t_env *e)
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
}

void				print_current_stats(t_env *e)
{
	int		i;

	i = -1;
	attron(COLOR_PAIR(WRITING));
	printw("\n\nCYCLES %-30d", e->c_total);
	printw("CYCLE_TO_DIE %4d/%-30d", e->cycle, e->c_to_die);
	printw("PROCESSES %-30d", e->nb_proc);
	printw("LAST LIVE FROM PLAYER ");
	if (e->v.live_color)
		attron(COLOR_PAIR(e->v.live_color));
	printw("%-30d\n\n", e->last_live);
	while (++i < e->nb_champ)
	{
		attron(COLOR_PAIR(WRITING));
		printw(" Player %11d : ", e->champs[i].id);
		attron(COLOR_PAIR(e->champs[i].color));
		if (e->champs[i].nb_live <= 1)
			printw(" %60s %4d live\n", e->champs[i].name,
					e->champs[i].nb_live);
		else
			printw(" %60s %4d lives\n", e->champs[i].name,
					e->champs[i].nb_live);
	}
	printw("\n");
}

static void			print_progress_bar(t_env *e)
{
	int		i;
	double	size;
	double	total_size;

	i = -1;
	total_size = 0;
	printw("\n\n\n");
	while (++i < MEM_SIZE)
		if (e->v.color_map[i] != 6)
			e->v.posess[e->v.color_map[i] - 2]++;
	i = -1;
	while (++i < e->nb_champ)
	{
		attron(COLOR_PAIR(e->champs[i].color + 6));
		size = ((double)e->v.posess[i] / (double)MEM_SIZE) * 100;
		total_size += (int)size;
		while (size-- > 0.00)
			printw("#");
		e->v.posess[i] = 0;
	}
	attron(COLOR_PAIR(PROGRESS_NO));
	while ((int)total_size++ < 187)
		printw("#");
	printw("\n\n");
}

void				print_current_map(t_env *e)
{
	int		i;
	int		highlight;

	i = -1;
	erase();
	print_current_stats(e);
	while (++i < MEM_SIZE)
	{
		highlight = e->v.color_map[i] < 0 ? 1 : 0;
		e->v.color_map[i] = highlight ? -e->v.color_map[i] : e->v.color_map[i];
		attron(COLOR_PAIR(e->v.color_map[i]));
		if (!(i % 64) || i)
			!(i % 64) ? printw("\n") : printw(" ");
		highlight ? attron(A_STANDOUT) : 0;
		printw("%02x", e->mem[i]);
		highlight ? attroff(A_STANDOUT) : 0;
	}
	print_progress_bar(e);
	timeout(0);
	if (getch() == ' ')
		while (42)
			if (getch() == ' ')
				break ;
	refresh();
	usleep(10000);
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
	init_pair(PLAYER_3, 11, COLOR_BLACK);
	init_pair(PLAYER_4, 1, COLOR_BLACK);
	init_pair(NO_ONE, 242, COLOR_BLACK);
	init_pair(WRITING, 253, COLOR_BLACK);
	init_pair(PROGRESS_1, 32, 32);
	init_pair(PROGRESS_2, 28, 28);
	init_pair(PROGRESS_3, 11, 11);
	init_pair(PROGRESS_4, 1, 1);
	init_pair(PROGRESS_NO, 242, 242);
	print_current_map(e);
	while (getch() != ' ')
		;
}
