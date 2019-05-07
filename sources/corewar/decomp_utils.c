/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:53:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/07 13:56:28 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

//Need check_champ, check_magic_number, split_champ, structs t_champ, t_decomp, et t_buff_d;

void			init_line(t_env *e)
{
	e->d.x = 0;
	e->d.op = e->champs[e->d.champ].content[e->d.i];
	nb_in_buff(&e->d, e->d.i, 1);
	str_in_buff(&e->d, ":\t\t");
	str_in_buff(&e->d,
			g_op_tab[e->champs[e->d.champ].content[e->d.i++] - 1].label);
	e->d.buff_d->tab[e->d.y][e->d.x++] = ' ';
}

int				generate_decomp_file(t_decomp *d, char *arg)
{
	int			i;
	int			fd;
	char		*name;
	t_buff_d	*ptr;

	ptr = d->buff_d;
	if (!(name = ft_strjoin(arg, "_decomp")))
	{
		ft_dprintf(2, "{bold}{#ed000b}fatal error:{R} %s\n", strerror(errno));
		return (0);
	}
	fd = open(name, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (!fd || fd == -1)
	{
		ft_dprintf(2, "{bold}{#ed000b}%s{#ffffff} error:{R} %s\n", arg,
				strerror(errno));
		free(name);
		return (0);
	}
	while (ptr)
	{
		i = 0;
		while (*ptr->tab[i])
			ft_dprintf(fd, "%s\n", ptr->tab[i++]);
		ptr = ptr->next;
	}
	free(name);
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
	while (d->x >= (int)tmp && padding)
		d->buff_d->tab[d->y][--d->x] = '0';
	d->x = padding ? tmp + 6 : tmp + len;
}

void			str_in_buff(t_decomp *d, char *s)
{
	while (*s)
		d->buff_d->tab[d->y][d->x++] = *s++;
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
