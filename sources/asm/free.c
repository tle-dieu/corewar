/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:04:37 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/22 16:37:14 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	free_line(t_file *file)
{
	while (file->begin)
	{
		file->last = file->begin->next;
		if (file->begin->free)
		{
			free(file->begin->s);
			free(file->begin);
		}
		else
			file->begin->next = NULL;
		file->begin = file->last;
	}
	file->last = NULL;
	file->begin = NULL;
}

void	free_lst_file(t_env *e)
{
	t_file *file;
	t_file *next;

	file = e->file;
	while (file)
	{
		next = file->next;
		free_line(file);
		free(file);
		file = next;
	}
	e->file = NULL;
}
