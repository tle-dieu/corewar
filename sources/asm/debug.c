/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:50:06 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/08 15:11:15 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#define BLUE "{rgb(0,188,218)}"

void	print_option(t_file *option, char *s)
{
	ft_printf(BLUE"filename: {reset}%s\n", s);
	ft_printf(BLUE"  => annot: {reset}%d\n", option->annot);
	ft_printf(BLUE"  => disas: {reset}%d\n", option->disas);
	ft_printf(BLUE"  => dump:  {reset}%d\n", option->dump);
	ft_printf("\n");
}

void	print_files(t_file *file)
{
	ft_printf("{yellow}----- Print File -----{reset}\n");
	while (file)
	{
		print_option(file, file->name);
		file = file->next;
	}
}
