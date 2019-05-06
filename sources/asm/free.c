/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:04:37 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/06 22:21:33 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	free_label(t_label **label)
{
	t_call *next;
	t_call *call;

	if (*label)
	{
		call = (*label)->call;
		while (call)
		{
			next = call->next;
			free_line(&call->line, 1);
			free(call);
			call = next;
		}
		free((*label)->name);
		free(*label);
		*label = NULL;
	}
}

void	free_buff(t_file *file)
{
	t_buff *buff;
	t_buff *next;

	buff = file->begin_buff;
	while (buff)
	{
		next = buff->next;
		free(buff);
		buff = next;
	}
}

void	free_line(t_line **line, int label)
{
	if (*line)
	{
		if ((*line)->free + label == 1)
		{
			free((*line)->s);
			free(*line);
		}
		else if ((*line)->free == -1 && label)
			(*line)->free = 0;
		*line = NULL;
	}
}

void	free_file(t_file **file)
{
	t_label	*next;

	free((*file)->output);
	if ((*file)->last != (*file)->begin)
		free_line(&(*file)->last, 0);
	else
		(*file)->last = NULL;
	free_line(&(*file)->begin, 0);
	free_buff(*file);
	while ((*file)->label)
	{
		next = (*file)->label->next;
		free_label(&(*file)->label);
		(*file)->label = next;
	}
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
