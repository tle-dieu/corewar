/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/17 18:10:16 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h> // tmp
#include <unistd.h>

//verifier que asm passe seulement ' ' et '\t'

int		add_line(t_env *e, char **line)
{
	t_line	*new;
	int		ret;

	*line = NULL;
	if ((ret = get_next_line(e->actual->fd, line)) <= 0)
		return (ret == -1 ? alloc_error(e) : 0);
	if (!(new = (t_line *)malloc(sizeof(t_line))))
	{
		free(*line);
		alloc_error(e);
	}
	ft_printf(COMMENT_C"line: %s\n{R}", *line);
	new->next = NULL;
	new->s = *line;
	if (!e->actual->begin)
	{
		new->y = ++e->line_nb;
		e->actual->begin = new;
	}
	else
	{
		new->y = ++e->line_nb;
		e->file->last->next = new;
	}
	e->actual->last = new;
	return (1);
}

void	compile(t_env *e)
{
	unsigned char	*cp;
	unsigned char	bin[BIN_MAX_SIZE];
	int				i;

	ft_printf("{yellow}----------COMPILE----------\n{R}");
	ft_bzero(bin, BIN_MAX_SIZE);
	i = 4;
	cp = bin;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_header(e, cp);
	/* ft_printf(STR_C"file: %s\n", e->actual->name); */
	/* ft_printf(STR_C"error: %d\n", e->actual->error); */
	/* if (!e->actual->error) */
		/* print_bin(bin, BIN_MAX_SIZE); */
	/* ft_printf("\n"); // a retirer */
}

int		main(int ac, char **av)
{
	t_env	e;

	e = (t_env){1, isatty(2), 1, NULL, NULL, av[0]};
	if (ac < 2)
		return (usage(av[0], 3));
	if (!parse_command_line(&e, ac, av))
		return (1);
	if (!e.file)
		return (usage(av[0], 3));
	print_files(e.file);
	e.actual = e.file;
	while (e.actual)
	{
		compile(&e);
		e.actual = e.actual->next;
	}
	free_lst_file(e.file);
}
