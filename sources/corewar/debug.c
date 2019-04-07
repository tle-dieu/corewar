/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:33 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/07 19:48:45 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			print_chmp(t_env *e, int cursor)
{
	int		i;
	int		count;

	count = 0;
	ft_printf("\n\n=== CHMP ===\n");
	i = PROG_NAME_LENGTH + COMMENT_LENGTH + 16;
	while (i++ < MAX_SIZE - 1)
	{
		count++;
		(i == cursor) ? ft_printf(">> ") : 1;
		ft_printf("%02x ", e->line[i]);
		(i == cursor) ? ft_printf("<< ") : 1;
		if (count == 16)
		{
			ft_printf("\n");
			count = 0;
		}
	}
	ft_printf("\n");
}

static void		print_process(t_env *e)
{
	int reg;
	int champ;

	champ = -1;
	while (++champ < e->nb_champ)
	{
		reg = -1;
		ft_printf("{#009688}proc_id{#ffffff} = %d\n", e->champs[champ].proc->id);
		ft_printf("{#009688}live{#ffffff} = %d\n", e->champs[champ].proc->live);
		ft_printf("{#009688}registre:\n");
		while (++reg < 16)
			ft_printf("{#009688}[%d]{#ffffff} = %d\n", reg, e->champs[champ].proc->r[reg]);
		ft_printf("{#009688}pc{#ffffff} = %d\n", e->champs[champ].proc->pc);
		ft_printf("{#009688}carry{#ffffff} = %d\n", e->champs[champ].proc->carry);
		ft_printf("{#009688}op{#ffffff} = %d\n", e->champs[champ].proc->op);
		ft_printf("{#009688}cycle{#ffffff} = %d\n", e->champs[champ].proc->cycle);
		ft_printf("{#009688}next{#ffffff} = %p\n\n", e->champs[champ].proc->next);
	}
}

void			print_env(t_env e)
{
	int		i;
	int		j;
	int		color;
	char	colors[4][12] = {"{#ff3333}\0", "{yellow}", "{blue}\0", "{yellow}\0"};

	i = -1;
	print_process(&e);
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
				ft_printf("%02x ", e.mem[i]);
				color = 1;
			}
		}
		(!color) ? ft_printf("{#666666}%02x ", e.mem[i]) : 1;
	}
	ft_printf("{reset}\nCYCLES: %-10d CTD: %d / %-10d \n", e.c_total, e.cycle, e.c_to_die);
}

void		print_split_champ(t_env *e, int i)
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
