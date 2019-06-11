/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disass_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:53:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/06/11 01:53:59 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void			init_line(t_disass *d)
{
	d->x = 0;
	d->op = d->content[d->i];
	nb_in_buff(d, d->i, 1);
	str_in_buff(d, ":\t\t");
	str_in_buff(d, g_op_tab[d->content[d->i++] - 1].label);
	d->buff_d->tab[d->y][d->x++] = ' ';
}

void			nb_in_buff(t_disass *d, long int nb, int padding)
{
	unsigned int len;
	unsigned int abs;
	unsigned int tmp;

	len = (nb < 0) ? 2 : 1;
	abs = ((nb < 0) ? -nb : nb);
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

void			str_in_buff(t_disass *d, char *s)
{
	while (*s)
		d->buff_d->tab[d->y][d->x++] = *s++;
}

int				compute_param(t_disass *d, int i, int size)
{
	int		nb;

	nb = 0;
	while (size--)
		nb += (unsigned)d->content[i++] << (8 * size);
	return (nb);
}
