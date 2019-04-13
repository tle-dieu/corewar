/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:50:06 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/13 20:50:32 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"

void			print_option(t_file *option, char *s)
{
	ft_printf(STR_C"filename: {R}%s\n", s);
	ft_printf(STR_C"  => annot: {R}%d\n", option->annot);
	ft_printf(STR_C"  => disas: {R}%d\n", option->disas);
	ft_printf(STR_C"  => dump:  {R}%d\n", option->dump);
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

void	get_color(int i)
{
	if (i < 4)
		ft_printf(MAGIC_C);
	else if (i < PROG_NAME_LENGTH + 4)
		ft_printf(NAME_C);
	else if (i < PROG_NAME_LENGTH + 12 && i >= PROG_NAME_LENGTH + 8)
		ft_printf(SIZE_C);
	else if (i < COMMENT_LENGTH + PROG_NAME_LENGTH + 12 && i >= PROG_NAME_LENGTH + 12) 
		ft_printf(COMMENT_C);
	else if (i >= COMMENT_LENGTH + PROG_NAME_LENGTH + 16)
		ft_printf(CHAMP_C);
}

static void		print_ascii(unsigned char *buff, int j, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		get_color(j);
		ft_printf("%c{R}", (buff[j] >= 32 && buff[j] < 127) ? buff[j] : '.');
		j++;
		i++;
	}
	ft_printf("\n");
}

void	print_bin(unsigned char *buff, int size)
{
	int		j;

	j = 0;
	ft_printf(STR_C"name:{R} %s\n", buff + 4);
	ft_printf(STR_C"comment:{R} %s\n\n", buff + PROG_NAME_LENGTH + 12);
	while (j < size)
	{
		if (!(j % 16))
			ft_printf("%08x:{R} ", j);
		get_color(j);
		ft_printf("%02x{R}", buff[j]);
		if (j % 2)
			ft_printf(" ");
		if (!((j + 1) % 16))
			print_ascii(buff, j - 15, 16);
		j++;
	}
	ft_printf("%*c", (int)((16 - (j - 1) % 16 - 1) * 2.5), ' ');
	print_ascii(buff, j - (j - 1) % 16 - 1, (j - 1) % 16 + 1);
}
