/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:04:37 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/30 09:25:18 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	free_line(t_line **line)
{
	if (*line && (*line)->free)
	{
		ft_printf("free line: %p\n", (*line)->s);
		free((*line)->s);
		ft_printf("free line 2\n");
		free(*line);
		ft_printf("free line 3\n");
	}
	*line = NULL;
}

void	free_file(t_file **file)
{
	free((*file)->output);
	free_line(&(*file)->begin);
	if (!(*file)->uniq_line)
		free_line(&(*file)->last);
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
		file = next;
	}
	e->file = NULL;
}
