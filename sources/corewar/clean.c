/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:55:35 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/01 19:04:21 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			freedom(t_env *e, int to_exit)
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
	if (to_exit)
		exit(1);
}
