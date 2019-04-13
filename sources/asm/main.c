/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/14 01:00:06 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h> // tmp

//verifier que asm passe seulement ' ' et '\t'

int		add_line(char **line, t_file *file)
{
	t_line	*new;

	*line = NULL;
	if (get_next_line(file->fd, line) != 1)
	{
		free(*line);
		return (0);
	}
	if (!(new = (t_line *)malloc(sizeof(t_line))))
	{
		free(*line);
		return (0);
	}
	new->next = NULL;
	new->s = *line;
	if (!file->begin)
	{
		new->y = 1;
		file->begin = new;
	}
	else
	{
		new->y = file->last->y + 1;
		file->last->next = new;
	}
	file->last = new;
	return (1);
}


void	compile(t_file *file)
{
	unsigned char	*cp;
	unsigned char	bin[BIN_MAX_SIZE];
	int				i;

	ft_bzero(bin, BIN_MAX_SIZE);
	i = 4;
	cp = bin;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_header(file, cp);
	print_bin(bin, BIN_MAX_SIZE);
}

int		main(int ac, char **av)
{
	t_file	*file;
	t_file	*actual;

	if (ac < 2)
		return (usage(av[0], 0));
	if (!(file = parse_command_line(ac, av)))
		return (1);
	print_files(file);
	actual = file;
	while (actual)
	{
		compile(actual);
		actual = actual->next;
	}
	free_lst_file(file);
}
