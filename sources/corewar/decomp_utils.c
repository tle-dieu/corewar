/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 15:47:42 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/26 10:29:06 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			init_decomp(t_decomp *d, int champ)
{
	int		i;

	i = -1;
	d->i = 0;
	d->champ = champ;
	while (++i < 300)
		ft_bzero(d->tab[i], COMMENT_LENGTH);
}

void			init_line(t_env *e)
{
	e->d.x = 0;
	e->d.op = e->champs[e->d.champ].content[e->d.i];
	nb_in_buff(&e->d, e->d.i, 1);
	str_in_buff(&e->d, ":\t\t");
	str_in_buff(&e->d,
		g_op_tab[e->champs[e->d.champ].content[e->d.i++] - 1].label);
	e->d.tab[e->d.y][e->d.x++] = ' ';
}

void			print_tab(t_decomp *d)
{
	int		i;

	i = 0;
	while (*d->tab[i])
		ft_printf("%s\n", d->tab[i++]);
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
		d->tab[d->y][d->x--] = (abs % 10) + 48;
		abs /= 10;
	}
	d->tab[d->y][d->x] = (abs % 10) + 48;
	if (nb < 0)
		d->tab[d->y][--d->x] = '-';
	while (d->x >= (int)tmp && padding)
		d->tab[d->y][--d->x] = '0';
	d->x = padding ? tmp + 6 : tmp + len;
}

void			str_in_buff(t_decomp *d, char *s)
{
	while (*s)
		d->tab[d->y][d->x++] = *s++;
}

int				compute_param(t_env *e, int champ, int i, int size)
{
	int		nb;
	int		j;

	nb = 0;
	j = 0;
	while (size--)
	{
		nb *= 256;
		nb += e->champs[champ].content[i + j++];
	}
	return (nb);
}
