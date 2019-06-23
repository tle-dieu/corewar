/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:08:56 by acompagn          #+#    #+#             */
/*   Updated: 2019/06/23 12:41:22 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static int	check_padding(unsigned char *line, char *arg)
{
	int		i;
	int		k;

	i = PROG_NAME_LENGTH + 3;
	k = 4;
	while (1)
	{
		k = i < COMMENT_LENGTH ? 6 : 4;
		while (k--)
		{
			if (line[i++])
			{
				ft_dprintf(2,
					"{bold}{#ed000b}%s{#ffffff}: wrong separators{R}\n", arg);
				return (0);
			}
		}
		if (i > COMMENT_LENGTH)
			break ;
		i += COMMENT_LENGTH + 3;
	}
	return (1);
}

static int	split_champ(t_env *e, int i)
{
	ft_memcpy(e->champs[i].name, e->line + 4, PROG_NAME_LENGTH);
	e->champs[i].name[PROG_NAME_LENGTH] = '\0';
	ft_memcpy(e->champs[i].comment, e->line + PROG_NAME_LENGTH + 12,
		COMMENT_LENGTH);
	e->champs[i].comment[COMMENT_LENGTH] = '\0';
	ft_memcpy(e->champs[i].content, e->line + HEADER_SIZE, CHAMP_MAX_SIZE);
	e->champs[i].content[CHAMP_MAX_SIZE] = '\0';
	if (!e->champs[i].name[0])
		ft_memcpy(e->champs[i].name, "no name", 7);
	return (1);
}

static int	check_magic_number(t_env *e)
{
	int		b;
	int		i;

	b = 16;
	i = 0;
	if (e->line[0])
		return (0);
	while (b >= 0)
	{
		if (e->line[++i] != (COREWAR_EXEC_MAGIC >> b & 0xff))
			return (0);
		b -= 8;
	}
	return (1);
}

static int	check_champ_size(t_env *e, char *arg, int ret, int i)
{
	e->champs[i].size = swap_bytes4(*(unsigned *)(e->line
				+ PROG_NAME_LENGTH + 8));
	if (e->champs[i].size <= 0)
	{
		ft_dprintf(2, "{bold}Champion {#ed000b}%s{#ffffff} too small{R}\n",
			arg);
		return (0);
	}
	else if (ret > MAX_SIZE || e->champs[i].size > CHAMP_MAX_SIZE)
	{
		ft_dprintf(2, "{bold}Champion {#ed000b}%s{#ffffff} too big{R}\n",
			arg);
		return (0);
	}
	else if (ret - (NAME_COMM_SIZE + 16) != e->champs[i].size)
	{
		ft_dprintf(2,
			"{bold}Champion {#ed000b}%s{#ffffff} size does not match{R}\n",
			arg);
		return (0);
	}
	return (1);
}

int			check_champ(t_env *e, char *arg, int i)
{
	int		fd;
	int		ret;
	int		err;

	fd = open(arg, O_RDONLY);
	err = 0;
	if (fd == -1 && (err = 1))
	{
		ft_dprintf(2, "{bold}{#ed000b}%s{#ffffff} error:{R} %s\n", arg,
			strerror(errno));
		return (0);
	}
	!err ? ft_bzero(e->line, MAX_SIZE) : 1;
	ret = !err ? read(fd, e->line, MAX_SIZE + 1) : -1;
	close(fd);
	if (!err && (ret == -1 || !check_magic_number(e)) && (err = 1))
		ft_dprintf(2, "{bold}%s: {#ed000b}%s{R}\n",
				ret == -1 ? strerror(errno) : "Invalid magic", arg);
	if (!err && !(check_champ_size(e, arg, ret, i)))
		err = 1;
	if (err || !check_padding(e->line, arg) || (!split_champ(e, i)))
		return (0);
	return (1);
}
