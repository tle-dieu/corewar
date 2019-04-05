/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:08:56 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/05 12:34:39 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			check_instructions(t_env *e)
{
	int		i;
	int		tmp;
	int		inst;
	int		begin;
	void	(*ft_ptr[16])() = {live, ld, st, add, sub, and, or, xor,
		zjmp, ldi, sti, op_fork, lld, lldi, lfork, aff};

	i = COMMENT_LENGTH + PROG_NAME_LENGTH + 16;
	inst = 0;
	begin = i;
	while (e->line[i] - 1 >= 0 && e->line[i] - 1 < 16)
	{
		tmp = i;
		(*ft_ptr[e->line[i] - 1])(e, &i);
		inst++;
		if (i == tmp)
		{
			printf("ERROR :: wrong instruction (number %d, opcode %d)\n",
					inst, e->line[i] - 1);
			break ;
		}
		printf("\n");
	}
	print_chmp(e, 0, 1, i);
	printf("instructions = %d\nchmp_size = %d\ni count = %d", inst, e->chmp_size, i - begin);
}

int			check_magic_number(t_env *e)
{
	int		b;
	int		i;

	b = 16;
	i = 0;
	while (b >= 0)
	{
		if (e->line[++i] != (COREWAR_EXEC_MAGIC >> b & 0xff))
			return (0);
		b -= 8;
	}
	return (1);
}

int			check_chmp_size(t_env *e, int ret)
{
	//printf("%02x + %02x > %d\n", e->line[PROG_NAME_LENGTH + 10],
	//		e->line[PROG_NAME_LENGTH + 11], CHAMP_MAX_SIZE);
	if (ret > MAX_SIZE || e->line[PROG_NAME_LENGTH + 10]
			+ e->line[PROG_NAME_LENGTH + 11] > CHAMP_MAX_SIZE)
		return (0);
	else
	{
		e->chmp_size = e->line[PROG_NAME_LENGTH + 10]
			+ e->line[PROG_NAME_LENGTH + 11];
		return (1);
	}
}
