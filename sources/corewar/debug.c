/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:33 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/11 21:06:45 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

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

void			print_process(t_env *e)
{
	int		i;
	t_proc	*ptr;

	i = -1;
	ft_printf("\n\n>>> PRINT PROCESS - CYCLE %d <<<\n", e->cycle);
	while (++i < e->nb_champ)
	{
		ptr = e->champs[i].proc;
		while (ptr)
		{
			ft_printf("Player %s(%d) :: pc = %-10d live = %-10d op = %-10d cycle = %-10d\n",
					e->champs[i].name, e->champs[i].id, ptr->pc, ptr->live, ptr->op, ptr->cycle);
			ptr = ptr->next;
		}
	}
	ft_printf("\n");
}

void			print_env(t_env e, int cursor)
{
	int		i;
	int		j;
	int		color;
	char	colors[4][12] = {"{#ff3333}\0", "{yellow}", "{blue}\0", "{yellow}\0"};

	i = -1;
	while (++i < MEM_SIZE)
	{
		j = -1;
		color = 0;
		(i % 64 == 0) ? ft_printf("\n") : 1;
		while (++j < e.nb_champ)
		{
			if (i >= e.champs[j].proc->pc && i <= e.champs[j].proc->pc + (int)e.champs[j].size)
			{
				ft_printf(colors[j]);
				(i == cursor) ? ft_printf(">>> ") : 1;
				ft_printf("%02x ", e.mem[i]);
				(i == cursor) ? ft_printf("<<< ") : 1;
				color = 1;
			}
		}
		(i == cursor) ? ft_printf(">>> ") : 1;
		(!color) ? ft_printf("{#666666}%02x ", e.mem[i]) : 1;
		(i == cursor) ? ft_printf("<<< ") : 1;
	}
	ft_printf("{reset}\nCYCLES: %-10d CTD: %d / %-10d \n", e.c_total, e.cycle, e.c_to_die);
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
