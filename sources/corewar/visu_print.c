/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 10:50:50 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/17 15:56:13 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <ncurses.h>
#include <unistd.h>

static void			print_current_stats(t_env *e)
{
	int		i;

	i = -1;
	attron(COLOR_PAIR(WRITING));
	printw("\n\nCYCLES %-30d", e->c_total);
	printw("CYCLE_TO_DIE %4d/%-30d", e->cycle, e->c_to_die);
	printw("PROCESSES %-30d", e->nb_proc);
	printw("LAST LIVE FROM PLAYER ");
	e->v.live_color ? attron(COLOR_PAIR(e->v.live_color)) : 1;
	printw("%-30d", e->last_live);
	attron(COLOR_PAIR(WRITING));
	printw("     +/- SPEED\n\n");
	while (++i < e->nb_champ)
	{
		attron(COLOR_PAIR(WRITING));
		printw(" Player %11d : ", e->champs[i].id);
		attron(COLOR_PAIR(e->champs[i].color));
		if (e->champs[i].nb_live <= 1)
			printw(" %128s %5d live\n", e->champs[i].name,
				e->champs[i].nb_live);
		else
			printw(" %128s %5d lives\n", e->champs[i].name,
				e->champs[i].nb_live);
	}
	printw("\n");
}

static void			get_posessions(t_env *e)
{
	int		i;

	i = -1;
	while (++i < MEM_SIZE)
		if (e->v.map[i] != 6)
		{
			if (e->v.map[i] < 0)
				e->v.posess[-e->v.map[i] - 2]++;
			else
				e->v.posess[e->v.map[i] - 2]++;
		}
}

static void			print_progress_bar(t_env *e)
{
	int		i;
	double	size;
	int		total_size;

	i = -1;
	total_size = 0;
	printw("\n\n\n");
	get_posessions(e);
	while (++i < e->nb_champ)
	{
		attron(COLOR_PAIR(e->champs[i].color + 6));
		size = ((double)e->v.posess[i]
			/ (double)MEM_SIZE) * (double)PROGRESS_BAR_SIZE;
		total_size += (int)size;
		while ((int)size-- > 0)
			printw("#");
		e->v.posess[i] = 0;
	}
	attron(COLOR_PAIR(PROGRESS_NO));
	while (total_size++ < (int)PROGRESS_BAR_SIZE)
		printw("#");
	printw("\n");
	attroff(COLOR_PAIR(PROGRESS_NO));
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
		highlight = e->v.map[i] < 0 ? 1 : 0;
		e->v.map[i] = highlight ? -e->v.map[i] : e->v.map[i];
		attron(COLOR_PAIR(e->v.map[i]));
		if (!(i % 64) || i)
			!(i % 64) ? printw("\n") : printw(" ");
		highlight ? attron(A_STANDOUT) : 0;
		printw("%02x", e->mem[i]);
		highlight ? attroff(A_STANDOUT) : 0;
	}
	print_progress_bar(e);
	get_keys(e);
	refresh();
	usleep(e->v.sleep_value);
}
