/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 16:36:26 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/14 11:51:12 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>

int		main(int ac, char **av)
{
	t_env	e;
	t_file	*next;

	e = (t_env){isatty(1), isatty(2), 0, NULL, NULL, av[0], NULL, NULL};
	if (ac < 2)
		return (usage(&e, 3));
	if (!parse_command_line(&e, ac, av))
		return (!e.file ? usage(&e, 3) : 1);
	e.actual = NULL;
	if (!e.file)
		return (usage(&e, 3));
	while (e.file)
	{
		ft_printf("{yellow}file:{R} %s\n", e.file->name);
		if (e.file->options & O_DISAS)
			disassemble_champ(&e);
		else
			assemble(&e);
		next = e.file->next;
		free_file(&e.file);
		e.file = next;
	}
}
