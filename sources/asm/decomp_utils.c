/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:53:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/09 23:50:16 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

void			init_line(t_decomp *d)
{
	d->x = 0;
	d->op = d->content[d->i];
	nb_in_buff(d, d->i, 1);
	str_in_buff(d, ":\t\t");
	str_in_buff(d, g_op_tab[d->content[d->i++] - 1].label);
	d->buff_d->tab[d->y][d->x++] = ' ';
}

int				generate_decomp_file(t_decomp *d, t_buff_d *ptr, t_file *file)
{
	int			i;
	int			fd;
	char		*name;

	if (!(name = ft_strjoin(file->name, "_decomp")) && ft_dprintf(2,
				"{bold}{#ed000b}fatal error:{R} %s\n", strerror(errno)))
		return (free_buff_decomp(d));
	fd = open(name, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd == -1 && ft_dprintf(2,
		"{bold}{#ed000b}%s{#ffffff} error:{R} %s\n", file->name,
		strerror(errno)))
	{
		free(name);
		return (free_buff_decomp(d));
	}
	while (ptr && (i = -1))
	{
		while (*ptr->tab[++i])
			ft_dprintf(fd, "%s\n", ptr->tab[i]);
		ptr = ptr->next;
	}
	free(name);
	free_buff_decomp(d);
	return (1);
}

void			nb_in_buff(t_decomp *d, int nb, int padding)
{
	unsigned int	len;
	unsigned int	abs;
	unsigned int	tmp;

	len = (nb < 0) ? 2 : 1;
	abs = (nb < 0) ? -nb : nb;
	tmp = abs;
	while (tmp > 9)
	{
		len++;
		tmp /= 10;
	}
	tmp = d->x;
	d->x += padding ? 6 - 1 : len - 1;
	while (abs > 9)
	{
		d->buff_d->tab[d->y][d->x--] = (abs % 10) + 48;
		abs /= 10;
	}
	d->buff_d->tab[d->y][d->x] = (abs % 10) + 48;
	if (nb < 0)
		d->buff_d->tab[d->y][--d->x] = '-';
	while (d->x > (int)tmp && padding)
		d->buff_d->tab[d->y][--d->x] = '0';
	d->x = padding ? tmp + 6 : tmp + len;
}

void			str_in_buff(t_decomp *d, char *s)
{
	while (*s)
		d->buff_d->tab[d->y][d->x++] = *s++;
}

int				compute_param(t_decomp *d, int i, int size)
{
	int		nb;
	int		j;

	nb = 0;
	j = 0;
	while (size--)
	{
		nb *= 256;
		nb += d->content[i + j++];
	}
	return (nb);
}
