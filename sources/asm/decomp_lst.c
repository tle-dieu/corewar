/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp_lst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 22:43:05 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/10 13:12:01 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

// autres malloc ?

int					free_buff_decomp(t_decomp *d)
{
	t_buff_d	*ptr;
	t_buff_d	*tmp;

	ptr = d->main_ptr;
	if (d->content) // pas besoin de checker si NULL
		free(d->content);
	d->content = NULL;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return (0);
}

int					add_buff_link(t_env *e, t_decomp *d)
{
	t_buff_d	*new;
	t_buff_d	*ptr;
	int			i;

	i = -1;
	d->y = 0;
	if (!(new = (t_buff_d*)malloc(sizeof(t_buff_d))))
	{
		free_buff_decomp(d);
		alloc_error(e);
	}
	new->next = NULL;
	while (++i < BS_DECOMP)
		ft_bzero(new->tab[i], COMMENT_LENGTH + 11);
	if (!d->buff_d)
	{
		d->buff_d = new;
		d->main_ptr = new;
	}
	else
	{
		ptr = d->buff_d;
		while (ptr && ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		d->buff_d = ptr->next;
	}
	return (1);
}
