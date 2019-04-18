/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:33 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/18 11:39:53 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			print_game(t_env *e, t_proc *ptr)
{
	if (ptr->owner == -1)
		ft_printf("%d :: {#0bd185}%-25d ===> OP %-10d %-10d PC %-10d{reset}\n",
				e->c_total, ptr->owner, ptr->op, ptr->id, ptr->pc);
	else
		ft_printf("%d :: {#f4428c}%-25d ===> OP %-10d %-10d PC %-10d{reset}\n",
				e->c_total, ptr->owner, ptr->op, ptr->id, ptr->pc);
}

void			print_chmp(t_env *e, int c, unsigned int cursor)
{
	unsigned int		i;
	unsigned int		count;

	count = 0;
	ft_printf("\n\n=== %s %d ===\n", e->champs[c].name, e->champs[c].id);
	i = 0;
	while (i++ < e->champs[c].size)
	{
		count++;
		(i == cursor) ? ft_printf(">> ") : 1;
		ft_printf("%02x ", e->champs[c].content[i]);
		(i == cursor) ? ft_printf("<< ") : 1;
		if (count == 16)
		{
			ft_printf("\n");
			count = 0;
		}
	}
	ft_printf("\n");
}

void			print_split_champ(t_env *e, int i)
{
	int		j;
	int		count;

	j = -1;
	ft_printf("NAME\n%s\n\n", e->champs[i].name);
	ft_printf("COMMENT\n%s\n\n", e->champs[i].comment);
	ft_printf("CONTENT\n");
	while (++j < CHAMP_MAX_SIZE)
	{
		ft_printf("%02x ", e->champs[i].content[j]);
		if (count == 16)
		{
			count = 0;
			ft_printf("\n");
		}
		count++;
	}
	ft_printf("\n\n");
}
