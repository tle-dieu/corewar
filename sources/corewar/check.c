/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:08:56 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/28 15:58:43 by acompagn         ###   ########.fr       */
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
	e->champs[i].size = e->line[PROG_NAME_LENGTH + 10] * 256
		+ e->line[PROG_NAME_LENGTH + 11];
	return (ret < MAX_SIZE || e->champs[i].size < CHAMP_MAX_SIZE
			|| e->champs[i].size < 0);
}

int				check_champ(t_env *e, char *arg, int i)
{
	int		fd;
	int		ret;
	int		err;

	fd = open(arg, O_RDONLY);
	err = 0;
	if ((fd == -1 || !fd) && (err = 1))
		ft_dprintf(2, "\nWrong .cor file: %s\n", arg);
	!err ? ft_bzero(e->line, MAX_SIZE) : 1;
	ret = !err ? read(fd, e->line, MAX_SIZE + 1) : -1;
	if (!err && (ret == -1 || !(check_magic_number(e))) && (err = 1))
	{
		if (ret == -1)
			ft_dprintf(2, "\nNot a valid file: %s\n", arg);
		else
			ft_dprintf(2, "\nDoes not contain magic number\n");
	}
	if (!err && !(check_champ_size(e, ret, i)) && (err = 1))
		ft_dprintf(2, "\nChampion %s too big (%d > %d)\n", arg,
			e->champs[i].size, CHAMP_MAX_SIZE);
	if (err)
		return (0);
	split_champ(e, i);
	decompile_champ(e, arg, i);
	return (1);
}
