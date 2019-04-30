/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:04:37 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/30 14:53:09 by tle-dieu         ###   ########.fr       */
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
	if (PRINT)
	{
		ft_printf("file->output write: %s\n", (*file)->output);
		ft_printf("{yellow}in free_file :: {R}%p\n", (*file)->begin);
		ft_printf("output: %p\n", (*file)->output);
		ft_printf("{purple}{bold}file addr: %p\n{R}", *file);
		ft_printf("last: %p != begin: %p\n", (*file)->last, (*file)->begin);
	}
	free((*file)->output);
	if ((*file)->last != (*file)->begin)
	{
		if (PRINT)
			ft_printf("{green}{bold}before (*file)->last {R}%p\n", (*file)->last);
		free_line(&(*file)->last);
		if (PRINT)
			ft_printf("{bold}{green}after (*file)->last {R}%p\n", (*file)->last);
	}
	else
		(*file)->last = NULL;
	if (PRINT)
		ft_printf("{purple}{bold}before (*file)->begin {R}%p\n", (*file)->begin);
	free_line(&(*file)->begin);
	if (PRINT)
		ft_printf("{bold}{purple}after (*file)->begin {R}%p\n", (*file)->begin);
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
