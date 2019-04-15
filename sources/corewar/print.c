/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:37:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/15 13:54:27 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		usage(char *path)
{
	if (path)
		ft_printf("usage:\n	%s [-dump nbr_cycles] [[-n number] champion1.cor] ...\n	You must enter between 2 and 4 champions\n	Champions must have the .cor extension\n\n", path);	
}

void		print_memory(t_env *e)
{
	int		i;

	i = -1;
	while (++i < MEM_SIZE)
	{
		if (i && !(i % 64))
			ft_printf("\n", e->mem[i]);
		ft_printf("%02x ", e->mem[i]);
	}	
}

void		print_winner(t_env *e)
{
	int		i;

	i = -1;
	ft_printf("\n\n");
	if (!e->total_live)
		ft_printf("NOBODY WINS\n");
	else
	{
		while (++i < e->nb_champ)
		{
			if (e->last_live == e->champs[i].id)
				ft_printf("WINNER {#0bd185}%s(%d){reset}\n", e->champs[i].name, e->champs[i].id);
			else
				ft_printf("LOOSER {#f48042}%s(%d){reset}\n", e->champs[i].name, e->champs[i].id);
		}
		ft_printf("\n\n");
	}
}

void		aff(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;
	int		p;

	p = e->mem[(*pc + 2) % MEM_SIZE];
	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (check.p1 == 64 && check_reg(p) && !check.p2 && !check.p3)
		ft_printf("%c", ptr->r[p] % 256);
	*pc += 3;
}
