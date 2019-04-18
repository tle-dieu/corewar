/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/18 18:42:50 by acompagn         ###   ########.fr       */
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
		e->color_map[i] = 6;
	while (++champ < e->nb_champ)
	{
		j = -1;
		i = champ * (MEM_SIZE / e->nb_champ);
		while (++j < e->champs[champ].size)
			e->color_map[i++] = champ + 2;
	}
}

void				print_current_stats(t_env *e)
{
	int		i;

	i = -1;
	attron(COLOR_PAIR(WRITING));
	printw("\nCYCLES %-10d", e->c_total);
	printw("CYCLE_TO_DIE %4d/%4d       ", e->cycle, e->c_to_die);
	printw("PROCESSES %d      ", e->nb_proc);
	printw("LAST LIVE FROM PLAYER ");
	if (e->live_color)
		attron(COLOR_PAIR(e->live_color));
	printw("%2d\n", e->last_live);
	while (++i < e->nb_champ)
	{
		attron(COLOR_PAIR(WRITING));
		printw("Player %d : ", e->champs[i].id);
		attron(COLOR_PAIR(e->champs[i].color));
		printw("%s\n", e->champs[i].name);	
	}
	printw("\n");
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
		highlight = 0;
		if (e->color_map[i] < 0)
		{
			highlight = 1;
			e->color_map[i] = -e->color_map[i];
		}
		attron(COLOR_PAIR(e->color_map[i]));
		if (i && !(i % 64))
			printw("\n");
		else if (i)
			printw(" ");
		highlight ? attron(A_STANDOUT) : 0;
		printw("%02x", e->mem[i]);
		highlight ? attroff(A_STANDOUT) : 0;
	}
	refresh();
	timeout(0);
	if (getch() == ' ')
		while (42)
			if (getch() == ' ')
				break ;
	usleep(10000);
}

void				visu(t_env *e)
{
	initscr();
	curs_set(0);
	noecho();
	if (has_colors() == FALSE)
	{
		endwin();
		exit(1);
	}
	start_color();
	e->live_color = 0;
	init_color_map(e);
	init_pair(PLAYER_1, 21, COLOR_BLACK);
	init_pair(PLAYER_2, 88, COLOR_BLACK);
	init_pair(PLAYER_3, COLOR_GREEN, COLOR_BLACK);
	init_pair(PLAYER_4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(NO_ONE, 242, COLOR_BLACK);
	init_pair(WRITING, 253, COLOR_BLACK);
	print_current_map(e);
	while (getch() != ' ')
			;
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
		if (!(check_champ(&e, av[e.champs[i].file], i, av)))
			return (1);
	}
	attribute_id(&e);
	place_champ(&e);
	if (VISU)
		visu(&e);
	play(&e);
	if (!VISU)
		print_winner(&e);
	freedom(&e);
	return (0);
}
