/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:37:56 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/06 00:07:33 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <errno.h>
#include <stdlib.h>

int		usage(t_env *e, int err)
{
	if (err == 3)
	{
		ft_dprintf(--err, line_error(ERR_ARGS, e->tty2), e->exname);
		ft_dprintf(err, "no input files\n");
	}
	ft_dprintf(err, "usage: %s [-a] [-x] sourcefile.s\n", e->exname);
	ft_dprintf(err, "       %s [-d] sourcefile.cor\n", e->exname);
	if (err == 1)
	{
		ft_printf("HELP\n"); // help a faire
		exit(0);
	}
	ft_dprintf(err, "Try `%s (-h | --help)' for more information.\n",
		e->exname);
	return (1);
}

int		error_file(t_env *e, char *error, char *file, unsigned opt)
{
	free_lst_file(e);
	ft_dprintf(2, line_error(ERR_ARGS, e->tty2), e->exname);
	if (opt & ~0xff)
	{
		if (opt & O_SHORT_ERR)
			ft_dprintf(2, "unknow option - %c\n", *error);
		else if (opt & O_LONG_ERR)
			ft_dprintf(2, "unknow option -- %s\n", error);
		else if (opt & (O_COL_AMBIGUOUS_ERR | O_COL_INVALID_ERR))
		{
			ft_dprintf(2, "%s argument '%s' for '--color'\n",
				opt & O_COL_AMBIGUOUS_ERR ? "ambiguous" : "invalid", error);
			ft_dprintf(2, "Valid arguments are:\n  - 'always', 'yes', 'force'\n\
				- 'never', 'no', 'none'\n  - 'auto', 'tty', 'if-tty'\n");
		}
		else if (opt & O_INVALID_FILE_ERR)
			ft_dprintf(2, "invalid file '%s'\n", file);
		else
			ft_dprintf(2, "argument to '-o' is missing\n");
	}
	else
		ft_dprintf(2, "%s: '%s'\n", strerror(errno), file);
	return (0);
}
