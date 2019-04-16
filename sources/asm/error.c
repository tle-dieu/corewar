/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:38:33 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/16 16:17:10 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <errno.h>

void    *alloc_error(char *ex_name)
{
    ft_printf("%s: "RED_ERR"error: {R}", ex_name);
    ft_printf("%s\n", strerror(errno));
    return (NULL);
}

int		error_header(t_file *file, int error, char *extra, int cmd)
{
	char *scmd;

	scmd = cmd ? COMMENT_CMD_STRING : NAME_CMD_STRING;
	file->error = 1;
	if (error == 1)
	{
		ft_dprintf(2, "{bold}%s:%d:%d: "RED_ERR"error: {R}{bold}unexpected expression after %s declaration{R}\n", file->name, file->last->y, extra - file->last->s + 1, scmd);
		err_pointer(file->last->s, extra);
	}
	else if (error == 2)
	{
		ft_dprintf(2, "{bold}%s:%d:%d: "RED_ERR"error: {R}{bold}%s declaration too long (Max length: %d){R}\n", file->name, file->begin->y, extra - file->begin->s + 1, scmd, cmd ? COMMENT_LENGTH : PROG_NAME_LENGTH);
		err_pointer(file->begin->s, extra++);
		err_wave(extra);
	}
	else if (error == 3)
	{
		ft_dprintf(2, "{bold}%s:%d:%d: "RED_ERR"error: {R}{bold}expected string after %s{R}\n", file->name, file->begin->y, extra - file->begin->s + 1, scmd);
		err_pointer(file->begin->s, extra);
	}
	ft_putchar('\n');
	return (!error);
}
