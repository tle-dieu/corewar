/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:37:56 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/13 14:39:04 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <errno.h>
#include <stdlib.h>

int		usage(char *ex_name, int help)
{
	ft_printf("usage: %s [-a] [-x] sourcefile.s\n", ex_name);
	ft_printf("       %s [-d] sourcefile.cor\n", ex_name);
	if (help)
	{
		ft_printf("HELP\n"); // help a faire
		exit(0);
	}
	ft_printf("Try `%s (-h | --help)' for more information.\n", ex_name);
	return (0);
}

int		error_file(t_file *option, char *ex_name, char *file, t_file *lst)
{
	free_lst_file(lst);
	ft_printf("%s: "RED_ERR"error: {R}", ex_name);
	if (option->error || option->name)
	{
		if (option->error)
			ft_printf("unknow option - %c\n", option->error);
		else
			ft_printf("unknow option -- %s\n", option->name);
		usage(ex_name, 0);
	}
	else
		ft_printf("%s: '%s'\n", strerror(errno), file);
	return (1);
}
