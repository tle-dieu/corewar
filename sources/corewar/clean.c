/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:55:35 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/18 22:05:03 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			freedom(t_env *e)
{
	t_proc	*ptr;
	t_proc	*tmp;

	ptr = e->proc;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
}

void			destroy_process(t_env *e, t_proc *to_del)
{
	t_proc	*ptr;
	t_proc	*tmp;

	ptr = e->proc;
	if (e->visu)
		ft_printf("\a");
	e->nb_proc--;
	if (ptr == to_del)
	{
		tmp = ptr;
		e->proc = ptr->next;
		free(tmp);
		return ;
	}
	while (ptr && ptr->next)
	{
		if (ptr->next == to_del)
		{
			tmp = ptr->next;
			ptr->next = to_del->next;
			free(tmp);
			return ;
		}
		ptr = ptr->next;
	}
}
