/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:37:56 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/17 20:37:29 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <errno.h>
#include <stdlib.h>

int		usage(t_env *e, int err)
{
	if (err == 3)
	{
		ft_dprintf(--err, COLOR_FILE(e->tty), e->exname);
		ft_dprintf(err, "no input files\n");
	}
	ft_dprintf(err, "usage: %s [-a] [-x] sourcefile.s\n", e->exname);
	ft_dprintf(err, "       %s [-d] sourcefile.cor\n", e->exname);
	if (err == 1)
	{
		ft_printf("HELP\n"); // help a faire
		exit(0);
	}
	ft_dprintf(err, "Try `%s (-h | --help)' for more information.\n", e->exname);
	return (1);
}

int		error_file(t_env *e, char *error, char *file, unsigned opt)
{
	free_lst_file(e->file);
	ft_dprintf(2, COLOR_FILE(e->tty), e->exname);
	if (opt & (O_LONG_ERR | O_SHORT_ERR))
	{
		if (opt & O_SHORT_ERR)
			ft_dprintf(2, "unknow option - %c\n", *error);
		else
			ft_dprintf(2, "unknow option -- %s\n", error);
		usage(e, 2);
	}
	else
		ft_dprintf(2, "%s: '%s'\n", strerror(errno), file);
	return (0);
}
