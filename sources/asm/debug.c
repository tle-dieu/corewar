/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:50:06 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/12 22:06:59 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#define BLUE "{rgb(0,188,218)}"

void			print_option(t_file *option, char *s)
{
	ft_printf(BLUE"filename: {R}%s\n", s);
	ft_printf(BLUE"  => annot: {R}%d\n", option->annot);
	ft_printf(BLUE"  => disas: {R}%d\n", option->disas);
	ft_printf(BLUE"  => dump:  {R}%d\n", option->dump);
	ft_printf("\n");
}

void			print_files(t_file *file)
{
	ft_printf("{yellow}----- Print File -----{R}\n");
	while (file)
	{
		print_option(file, file->name);
		file = file->next;
	}
}

static void		print_ascii(unsigned char *buff, int ret)
{
	int i;

	i = 0;
	while (i++ < 16 - ret - 1)
		ft_printf(" ");
	i = 0;
	while (ret + i++ < 16)
		ft_printf("  ");
	i = 0;
	while (i < ret)
	{
		ft_printf("%c", (buff[i] >= 32 && buff[i] < 127) ? buff[i] : '.');
		i++;
	}
}

void print_bin(unsigned char *buff, int size)
{
	int		i;
	int		j;

	j = 0;
	while (j < size)
	{
		i = 0;
		ft_printf("%08x: ", j);
		while (i < 16 && i + j < size)
		{
			ft_printf("%02x", buff[i + j]);
			if (i + 2 + j < size)
				ft_printf("%02x", buff[i + 1 + j]);
			ft_printf(" ");
			i += 2;
		}
		print_ascii(buff + j, i);
		j += i;
		ft_printf("\n");
	}
}
