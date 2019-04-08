/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:36:11 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/08 13:52:55 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

t_file  *add_file(t_file **file, t_file *last, char *name, t_file *option)
{
	t_file *new;

	if (!(new = (t_file *)malloc(sizeof(t_file))))
		return (NULL);
	new->next = NULL;
	new->name = name;
	new->annot = option->annot;
	new->dump = option->dump;
	new->disas = option->disas;
	new->error = 0;
	new->msg = NULL;
	if (*file)
		last->next = new;
	else
		*file = new;
	return (new);
}
