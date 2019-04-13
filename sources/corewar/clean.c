/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:55:35 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/13 14:11:39 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			freedom(t_env *e)
{
	ft_printf("free\n");
	(void)e;
}

void			destroy_process(t_env *e, int i, t_proc *to_del)
{
	t_proc	*ptr;
	t_proc	*tmp;

	ptr = e->champs[i].proc;
	e->champs[i].nb_proc--;
	if (ptr == to_del)
	{
		tmp = ptr;
		e->champs[i].proc = ptr->next;
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
		}
		ptr = ptr->next;
	}
}

void			destroy_all(t_env *e, int i)
{
	t_proc	*ptr;
	t_proc	*tmp;

	ptr = e->champs[i].proc;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	e->champs[i].proc = NULL;
}
