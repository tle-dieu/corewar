/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:55:35 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/13 12:48:11 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void		free_processes(t_env *e)
{
	t_proc	*ptr;
	t_proc	*tmp;

	if (e->new_proc)
	{
		e->proc_to_load->next = e->proc;
		e->proc = e->new_proc;
		e->new_proc = NULL;
	}
	ptr = e->proc;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	e->proc = NULL;
}

void			freedom(t_env *e, int to_exit)
{
	free_processes(e);
	if (to_exit)
	{
		ft_dprintf(2, "{bold}{#ed000b}fatal error:{R} %s\n", strerror(errno));
		exit(1);
	}
	if (e->visu && e->dump == -1)
	{
		while (getch() != ' ')
			;
		endwin();
	}
}
