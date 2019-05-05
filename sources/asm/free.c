/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:04:37 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/05 15:10:39 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	free_line(t_line **line)
{
	if (*line && (*line)->free)
	{
		free((*line)->s);
		free(*line);
	}
	*line = NULL;
}

void	free_file(t_file **file)
{
	free((*file)->output);
	if ((*file)->last != (*file)->begin)
		free_line(&(*file)->last);
	else
		(*file)->last = NULL;
	free_line(&(*file)->begin);
	free(*file);
	*file = NULL;
}

void	free_lst_file(t_env *e)
{
	t_file *file;
	t_file *next;

	file = e->file;
	while (file)
	{
		next = file->next;
		free_file(&file);
		file = next;
	}
	e->file = NULL;
}
