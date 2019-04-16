/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/16 23:50:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <errno.h>
#include <stdlib.h>

int		alloc_error(t_env *e)
{
    ft_printf("%s: "RED_ERR"error: {R}", e->exname);
    ft_printf("%s\n", strerror(errno));
	free_lst_file(e->file);
	exit(EXIT_FAILURE);
	return (-1);
}

int		error_header(t_file *file, int error, char *extra, int cmd)
{
	char	*scmd;
	t_line	*line;

	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	line = error == 1 ? file->last : file->begin;
	file->error = error || 0; //changer ca
	if (error)
		ft_dprintf(2, ERR_LINE_C, file->name, line->y, extra - line->s + 1);
	if (error == 1)
	{
		ft_dprintf(2, "unexpected expression after %s declaration{R}\n", scmd);
		err_pointer(file->last->s, extra);
	}
	else if (error == 2)
	{
		file->fatal_error = 1;
		ft_dprintf(2, "%s declaration too long (Max length: %d){R}\n", scmd, cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH);
		err_pointer(file->begin->s, extra++);
		err_wave(extra);
	}
	else if (error == 3)
	{
		ft_dprintf(2, "expected string after %s{R}\n", scmd);
		err_pointer(file->begin->s, extra);
	}
	ft_putchar('\n');
	return (!error);
}
