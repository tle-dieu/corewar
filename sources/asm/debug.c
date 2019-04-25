/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:50:06 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/25 22:19:26 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"

void	print_call_error(t_env *e)
{
	t_label *label;
	t_call	*call;

	label = e->actual->label;
	while (label)
	{
		if (label->index == -1)
		{
			call = label->call;
			ft_printf("{red}{bold}----- %s -----{R}\n", label->name);
			while (call)
			{
				ft_printf("{bold}%d:%d: {R}'%s'\n", call->line->y, call->s - call->line->s, call->line->s);
				// erreur print x
				call = call->next;
			}
		}
		label = label->next;
	}
}

void	print_label(t_env *e)
{
	t_label *label;

	label = e->actual->label;
	ft_printf("{yellow}---- Print Label ----{R}\n");
	ft_printf("actual index: %02x :: %d\n", e->actual->i, e->actual->i);
	while (label)
	{
		ft_printf(MAGIC_C"name: {R}%s"MAGIC_C" | index: {R}", label->name);
		if (label->index != -1)
			ft_printf("%02x "MAGIC_C"::{R} %d\n", label->index, label->index);
		else
			ft_printf("{#ff3333}missing{R}\n");
		label = label->next;
	}
}

void			print_option(unsigned options, char *s)
{
	ft_printf(STR_C"filename: {R}%s\n", s);
	ft_printf(STR_C"  => annot: {R}%s\n", options & O_ANNOT ? "yes" : "no");
	ft_printf(STR_C"  => disas: {R}%s\n", options & O_DISAS ? "yes" : "no");
	ft_printf(STR_C"  => dump:  {R}%s\n", options & O_DUMP ? "yes" : "no");
	ft_printf("\n");
}

void			print_files(t_file *file)
{
	ft_printf("{yellow}----- Print File -----{R}\n");
	while (file)
	{
		print_option(file->options, file->name);
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
