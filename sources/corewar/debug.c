/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:33 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 12:34:29 by acompagn         ###   ########.fr       */
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

void		print_env(t_env e)
{
	int i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (!(i % 64))
			ft_printf("\n");
		if ((int)e.mem[i] == 0)
			ft_printf("{#666666}%02x ", (int)e.mem[i]);
		else 
			ft_printf("{reset}%02x ", (int)e.mem[i]);
		i++;
	}
	ft_printf("{reset}\ntotal cycles: %-10d cyles to die: %d / %-10d \n", e.c_total, e.c, e.c_to_die);
}
