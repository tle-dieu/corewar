/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:08:56 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/07 19:40:46 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void		split_champ(t_env *e, int i)
{
	int		j;
	int		k;

	j = 3;
	k = 0;
	while (++j < MAX_SIZE)
	{
		if (j == PROG_NAME_LENGTH + 8 || j == PROG_NAME_LENGTH
				+ COMMENT_LENGTH + 16)
			k = 0;
		if (j < PROG_NAME_LENGTH + 8)
			e->champs[i].name[k++] = e->line[j];
		else if (j >= PROG_NAME_LENGTH + COMMENT_LENGTH + 16)
			e->champs[i].content[k++] = e->line[j];
		else if (j > PROG_NAME_LENGTH + 11)
			e->champs[i].comment[k++] = e->line[j];
	}
}

static int		check_instructions(t_env *e, int j)
{
	unsigned int	i;
	unsigned int	tmp;
	unsigned int	begin;
	int				inst;
	void			(*ft_ptr[16])() = {live, ld, st, add, sub, and,
		or, xor, zjmp, ldi, sti, op_fork, lld, lldi, lfork, aff};

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
			ft_printf("ERROR :: inst %d - op %d\n", inst, e->line[i] - 1);
			return (0);
		}
		ft_printf("\n");
	}
	if (i - begin != e->champs[j].size)
		return (0);
	return (1);
}

static int		check_magic_number(t_env *e)
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

static int		check_champ_size(t_env *e, int ret, int i)
{
	if (ret > MAX_SIZE || e->line[PROG_NAME_LENGTH + 10] * 256
			+ e->line[PROG_NAME_LENGTH + 11] > CHAMP_MAX_SIZE)
		return (0);
	else
	{
		e->champs[i].size = e->line[PROG_NAME_LENGTH + 10] * 256
			+ e->line[PROG_NAME_LENGTH + 11];
		return (1);
	}
}

int				check_champ(t_env *e, char *arg, int i)
{
	int		fd;
	int		ret;

	fd = open(arg, O_RDONLY);
	if (fd == -1 || !fd)
		return (0);
	ft_bzero(e->line, MAX_SIZE);
	ret = read(fd, e->line, MAX_SIZE + 1);
	if (!(check_champ_size(e, ret, i)))
	{
		ft_printf("Champ too big -> %d > %d\n",
				e->champs[i].size, CHAMP_MAX_SIZE);
		return (0);
	}
	if (!(check_magic_number(e)))
	{
		ft_printf("Wrong magic number\n");
		return (0);
	}
	if (!(check_instructions(e, i)))
		return (0);
	split_champ(e, i);
	print_chmp(e, -1);
	return (1);
}
