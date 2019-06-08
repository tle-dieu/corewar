/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 16:36:26 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/06/08 14:34:48 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>

static void		init_env(t_env *e, char **av)
{
	e->tty1 = isatty(1);
	e->tty2 = isatty(2);
	e->i = 0;
	e->actual = NULL;
	e->file = NULL;
	e->exname = av[0];
	e->output = NULL;
	e->d = NULL;
}

int				main(int ac, char **av)
{
	t_env	e;
	t_file	*next;

	init_env(&e, av);
	if (ac < 2)
		return (usage(&e, 3));
	if (!parse_command_line(&e, ac, av))
		return (!e.file ? usage(&e, 3) : 1);
	e.actual = NULL;
	if (!e.file)
		return (usage(&e, 3));
	while (e.file)
	{
		if (e.file->options & O_DISAS)
			disassemble_champ(&e);
		else
			assemble(&e);
		next = e.file->next;
		free_file(&e.file);
		e.file = next;
	}
	return (0);
}
