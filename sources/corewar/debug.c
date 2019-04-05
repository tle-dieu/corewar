/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:33 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 19:48:06 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		print_chmp(t_env *e, int full, int only_chmp, int cursor)
{
	int		i;
	int		count;
	int		size;

	i = -1;
	count = 0;
	size = 0;
	printf("=== CHMP ===\n");
	if (full)
		while (++i < MAX_SIZE)
		{
			if (i > 2140 && e->line[i] != 0)
				size++;
			printf("%02x ", e->line[i]);
		}
	else
	{
		while (++i < PROG_NAME_LENGTH + 8)
			if (!only_chmp)
				printf("%02x ", e->line[i]);
		if (!only_chmp)
			printf("\n");
		while (++i < COMMENT_LENGTH + PROG_NAME_LENGTH + 15)
			if (!only_chmp)
				printf("%02x ", e->line[i]);
		if (!only_chmp)
			printf("\n");
		while (i++ < MAX_SIZE - 1)
		{
			count++;
			if (i == cursor)
				printf(">> ");
			printf("%02x ", e->line[i]);
			if (i == cursor)
				printf("<< ");
			if (count == 16)
			{
				printf("\n");
				count = 0;
			}
		}
	}
	if (full)
		printf("size = %d\n", size);
	printf("\n");
}

void print_proc(t_env *e)
{
	int reg;
	int champ;

	champ = 0;
	while (champ < e->nb_champ)
	{
		reg = -1;
		ft_printf("{#009688}proc_id{#ffffff} = %d\n",e->champs[champ].proc->id);
		ft_printf("{#009688}proc_liv{#ffffff}e = %d\n",e->champs[champ].proc->live);
		ft_printf("{#009688}proc_registre:\n");
		while (++reg < 16)
			ft_printf("{#009688}[%d]{#ffffff} = %d\n", reg, e->champs[champ].proc->r[reg]);
		ft_printf("{#009688}proc_pc{#ffffff} = %d\n",e->champs[champ].proc->pc);
		ft_printf("{#009688}proc_carry{#ffffff} = %d\n",e->champs[champ].proc->carry);
		ft_printf("{#009688}proc_op{#ffffff} = %d\n",e->champs[champ].proc->op);
		ft_printf("{#009688}proc_cycle{#ffffff} = %d\n",e->champs[champ].proc->cycle);
		ft_printf("{#009688}proc_next{#ffffff} = %p\n",e->champs[champ].proc->next);
		champ++;
		ft_printf("\n");
	}
}

void		print_env(t_env e)
{
	int i;
	int champ;
	int color;
	char colors[4][12]={"{#ff3333}\0","{yellow}","{blue}\0", "{yellow}\0"};

	i = -1;
	print_proc(&e);
	while (++i < MEM_SIZE)
	{
		champ = -1;
		color = 0;
		if (i % 64 == 0)
			ft_printf("\n");
		while (++champ < e.nb_champ)
		{
			if (i >= e.champs[champ].proc->pc && i <= e.champs[champ].proc->pc + (int)e.champs[champ].size)
			{
				ft_printf(colors[champ]);
				ft_printf("%02x ", e.mem[i]);
				color = 1;
			}
		}
		if (!color)
			ft_printf("{#666666}%02x ",e.mem[i]);
	}
	ft_printf("{reset}\ntotal cycles: %-10d cycles to die: %d / %-10d \n", e.c_total, e.cycle, e.c_to_die);
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
