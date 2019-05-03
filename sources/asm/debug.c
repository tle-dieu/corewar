/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:50:06 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/03 04:10:18 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h>

void	print_call_error(t_env *e)
{
	t_label *label;
	t_call	*call;

	label = e->file->label;
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

	label = e->file->label;
	ft_printf("{yellow}---- Print Label ----{R}\n");
	ft_printf("actual index: %02x :: %d\n", e->file->i, e->file->i);
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

void			print_option(char *output, unsigned options, char *s)
{
	ft_printf(STR_C"filename: {R}%s\n", s);
	ft_printf(STR_C"  => hexa:   {R}%s\n", options & O_HEXA ? "yes" : "no");
	ft_printf(STR_C"  => binary: {R}%s\n", options & O_BIN ? "yes" : "no");
	ft_printf(STR_C"  => long:   {R}%s\n", options & O_LONG ? "yes" : "no");
	ft_printf(STR_C"  => disas:  {R}%s\n", options & O_DISAS ? "yes" : "no");
	if (output)
		ft_printf(STR_C"  => output: {R}yes: %s\n", output);
	else
		ft_printf(STR_C"  => output: {R}no\n", output);
	ft_printf("\n");
}

void			print_files(t_file *file)
{
	ft_printf("{yellow}----- Print File -----{R}\n");
	while (file)
	{
		print_option(file->output, file->options, file->name);
		file = file->next;
	}
}

int		get_color(int i, int last, int tty)
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

int		print_line(t_env *e, unsigned char *bin, int size, int i)
{
	int len;
	int j;
	int color;

	color = 0;
	len = e->file->options & O_HEXA ? 16 : 6;
	j = 0;
	if (i + j < size)
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
		ft_printf("%*c", (len == 6 ? 9 : 3) * (len - j) + (j < len / 2) + 1, ' ');
	j = 0;
	color = 0;
	while (i + j < size && j < len)
	{
		color = get_color(j + i, color, e->tty1);
		ft_printf("%c", (bin[j + i] >= 32 && bin[j + i] < 127) ? bin[j + i] : '.');
		j++;
	}
	if (e->tty1)
		ft_printf("{R}");
	ft_printf("\n");
	return (j);
}

int		pass_bytes(t_file *file, unsigned char *bin, int size, int i)
{
	int j;
	int len;

	j = 0;
	len = file->options & O_HEXA ? 16 : 6;
	while (!bin[j + i])
	{
		if (j + i >= size)
			return (-1);
		if (j == len)
			return (j);
		j++;
	}
	return (0);
}

unsigned char *lst_to_char(t_env *e, unsigned char *header, int *size)
{
	unsigned char	*str;	
	int				len;
	t_buff			*buff;
	int				i;
	int				j;

	buff = e->file->begin_buff;
	len = HEADER_SIZE;
	while (buff)
	{
		len += buff->len;
		buff = buff->next;
	}
	if (!(str = (unsigned char *)malloc(sizeof(unsigned char) * len)))
		return (NULL);
	*size = len;
	i = 0;
	while (i < HEADER_SIZE)
		str[i++] = *header++;
	buff = e->file->begin_buff;
	while (buff)
	{
		j = 0;
		while (j < buff->len)
			str[i++] = buff->s[j++];
		buff = buff->next;
	}
	return (str);
}
void	print_bin(t_env *e, unsigned char *header)
{
	int i;
	int	len;
	int	size;
	int	ret;
	int old;
	unsigned char *bin;

	i = 0;
	old = 0;
	ret = 0;
	bin = lst_to_char(e, header, &size);
	len = e->file->options & O_HEXA ? 16 : 6;
	while (i < size)
	{
		if (!(e->file->options & O_LONG)
		&& (ret = pass_bytes(e->file, bin, size, i)) == -1)
				break ;
		if (ret)
			i += ret;
		else
		{
			if (old)
				ft_printf("*\n");
			i += print_line(e, bin, size, i);
			i += print_line(e, bin, size, i);
		}
		old = ret;
	}
}
