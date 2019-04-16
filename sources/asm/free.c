/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:04:37 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/16 23:29:51 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	free_line(t_file *file)
{
	while (file->begin)
	{
		file->last = file->begin->next;
		free(file->begin->s);
		free(file->begin);
		file->begin = file->last;
	}
	file->last = NULL;
	file->begin = NULL;
}

void	free_lst_file(t_file *file)
{
	t_file *next;

	while (file)
	{
		next = file->next;
		free_line(file);
		free(file);
		file = next;
	}
}

