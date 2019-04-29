/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:37:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/29 12:21:29 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		usage(char *path)
{
	if (path)
		ft_printf("usage:\n\
			%s [-dump nbr_cycles] [[-n number]champion1.cor] ...\n\
			You must enter between 2 and 4 champions\n\
			Champions must have the .cor extension\n\n", path);
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
			ft_printf("{#f4428c}%02x {reset}", e->mem[i]);
		else if (!e->mem[i])
			ft_printf("{#63676d}%02x {reset}", e->mem[i]);
		else
			ft_printf("%02x ", e->mem[i]);
	}
	ft_printf("\n");
	freedom(e);
	exit(1);
}

void		print_subject_winner(t_env *e)
{
	int		i;

	i = -1;
	while (++i < e->nb_champ)
		if (e->last_live == e->champs[i].id)
			ft_printf("le joueur %d(%s) a gagne\n",
				e->champs[i].id, e->champs[i].name);
}

void		print_winner(t_env *e)
{
	int		i;

	i = -1;
	print_current_map(e);
	printw("\n\n");
	if (!e->total_live)
		printw("NOBODY WINS\n");
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
		}
		printw("\n\n");
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
