/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 01:31:18 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 17:43:43 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

static int	get_color(int i, int last, int tty)
{
	char	*color;
	int		code;

	if (!tty)
		return (0);
	code = 0;
	color = NULL;
	if (++code && i < 4)
		color = MAGIC_C;
	else if (++code && i < PROG_NAME_LENGTH + 4)
		color = NAME_C;
	else if (++code && i < PROG_NAME_LENGTH + 12 && i >= PROG_NAME_LENGTH + 8)
		color = SIZE_C;
	else if (++code && i < COMMENT_LENGTH + PROG_NAME_LENGTH + 12
	&& i >= PROG_NAME_LENGTH + 12 && ++code)
		color = COMMENT_C;
	else if (++code && i >= COMMENT_LENGTH + PROG_NAME_LENGTH + 16)
		color = CHAMP_C;
	else if (++code)
		color = "{R}";
	if (code != last)
		ft_printf(color);
	return (code);
}

static void	print_ascii(t_env *e, unsigned char *bin, int size, int i)
{
	char	c;
	int		color;
	int		j;
	int		len;

	j = 0;
	color = 0;
	len = e->file->options & O_HEXA ? 16 : 6;
	while (i + j < size && j < len)
	{
		color = get_color(j + i, color, e->tty1);
		c = (bin[j + i] >= 32 && bin[j + i] < 127) ? bin[j + i] : '.';
		ft_printf("%c", c);
		j++;
	}
	if (e->tty1)
		ft_printf("{R}");
	if (i < size)
		ft_printf("\n");
}

static int	print_line(t_env *e, unsigned char *bin, int size, int i)
{
	int len;
	int j;
	int color;

	color = 0;
	len = e->file->options & O_HEXA ? 16 : 6;
	j = 0;
	if (i < size)
		ft_printf("%08x:  ", i);
	while (i + j < size && j < len)
	{
		color = get_color(i + j, color, e->tty1);
		ft_printf(e->file->options & O_HEXA ? "%02x " : "%08b ", bin[i + j]);
		if (++j == len / 2)
			ft_printf(" ");
	}
	if (e->tty1)
		ft_printf("{R}");
	if (j)
		ft_printf("%*c",
			(len == 6 ? 9 : 3) * (len - j) + (j < len / 2) + 1, ' ');
	print_ascii(e, bin, size, i);
	return (j);
}

static int	pass_bytes(t_file *file, unsigned char *bin, int size, int i)
{
	int j;
	int len;

	j = 0;
	len = file->options & O_HEXA ? 16 : 6 - 1;
	while (!bin[j + i])
	{
		if (j + i >= size)
			return (-1);
		if (++j == len)
			return (j);
	}
	return (0);
}

void		dump_bytecode(t_env *e, unsigned char *header)
{
	int				i;
	int				size;
	int				ret;
	int				old;
	unsigned char	*bin;

	i = 0;
	old = 0;
	ret = 0;
	bin = lst_to_char(e, header, &size);
	while (i < size)
	{
		if (!(e->file->options & O_LONG)
			&& (ret = pass_bytes(e->file, bin, size, i)) == -1 && old)
			break ;
		if (old && !ret)
			ft_printf("*\n");
		if (!old || !ret)
			i += print_line(e, bin, size, i);
		else
			i += ret;
		old = ret;
	}
	free(bin);
}
