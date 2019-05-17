/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:37:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/17 16:08:28 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <ncurses.h>

void		usage(char *path, int help_option)
{
	if (path && !help_option)
		ft_printf("input {bold}{#ed000b}error{R}:\n");
	if (path)
		ft_printf("\
			{bold}%s{R} [-dump nbr_cycles] [[-n number]champion1.cor] [-v]...\n\
			You must enter between 2 and 4 champions\n\
			Champions must have the .cor extension\n\n\
			-dump to print memory at a precise cycle\n\
			-n to give a player number\n\
			-verbose to enable verbosity [0 to 4] (without viewer)\n\
			-v to enable viewer\n\n", path);
}

void		print_memory(t_env *e, int cursor)
{
	int		i;

	i = -1;
	while (++i < MEM_SIZE)
	{
		if (i && !(i % 32))
			ft_printf("\n", e->mem[i]);
		if (i && i == cursor)
			ft_printf("{#f4428c}%02x {R}", e->mem[i]);
		else if (!e->mem[i])
			ft_printf("{#63676d}%02x {R}", e->mem[i]);
		else
			ft_printf("%02x ", e->mem[i]);
	}
	ft_printf("\n");
}

void		print_subject_winner(t_env *e)
{
	int		i;

	i = -1;
	if (!e->total_live)
		ft_printf("Nobody wins\n");
	else
	{
		while (++i < e->nb_champ)
			if (e->last_live == e->champs[i].id)
				ft_printf("le joueur %d(%s) a gagne\n",
					e->champs[i].id, e->champs[i].name);
	}
}

void		print_winner(t_env *e)
{
	int		i;

	i = -1;
	print_current_map(e);
	printw("\n\n");
	if (!e->total_live)
		printw("%*s\n%*s\n", PROGRESS_BAR_SIZE, "NOBODY WINS",
			PROGRESS_BAR_SIZE, "PRESS SPACE TO EXIT");
	else
	{
		while (++i < e->nb_champ)
		{
			attron(COLOR_PAIR(e->champs[i].color));
			if (e->last_live == e->champs[i].id)
				printw("WINNER %40s(%d)\n",
					e->champs[i].name, e->champs[i].id);
			else
				printw("LOOSER %40s(%d)\n",
					e->champs[i].name, e->champs[i].id);
			attroff(COLOR_PAIR(e->champs[i].color));
		}
		printw("%*s\n\n", PROGRESS_BAR_SIZE, "PRESS SPACE TO EXIT");
	}
	refresh();
}

void		aff(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		p;

	p = e->mem[(*pc + 2) % MEM_SIZE];
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE],
			g_op_tab[ptr->op - 1].dir_size, ptr->op);
	if (!check.error && check_reg(p))
		ft_printf("%c", ptr->r[p] % 256);
	*pc += 3;
}
