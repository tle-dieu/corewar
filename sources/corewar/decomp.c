/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 10:26:51 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/22 18:31:20 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void			nb_in_buff(t_decomp *d, int nb)
{
	unsigned int	len;
	unsigned int 	abs;
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
	d->x += len - 1;
	while (abs > 9)
	{
		d->tab[d->y][d->x--] = (abs % 10) + 48;
		abs /= 10;
	}
	d->tab[d->y][d->x] = (abs % 10) + 48;
	if (nb < 0)
		d->tab[d->y][--d->x] = '-';
	d->x = tmp + len;
}

static void			str_in_buff(t_decomp *d, char *s)
{
	int		i;

	i = 0;
	while (s[i])
		d->tab[d->y][d->x++] = s[i++];
}

static int		compute_param(t_env *e, int champ, int i, int size)
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

static void			put_header(t_env *e, t_decomp *d)
{
	d->y = 0;
	d->x = 0;

	str_in_buff(d, ".name \"");
	str_in_buff(d, e->champs[d->champ].name);
	str_in_buff(d, "\"\n.comment \"");
	str_in_buff(d, e->champs[d->champ].comment);
	d->y++;
	d->tab[d->y][0] = '\n';
	d->y++;
}

static void			move_forward(t_env *e, t_decomp *d)
{
	d->tab[d->y][d->x++] = '%';
	if (d->op == 1)
	{
		nb_in_buff(d, compute_param(e, d->champ, d->i, 4));
		d->i += 4;
	}
	else if (d->op == 9 || d->op == 15 || d->op == 12)
	{
		nb_in_buff(d, (short)compute_param(e, d->champ, d->i, 2));
		d->i += 2;
	}
}

static void			param_to_str(t_env *e, t_decomp *d, int param, int size, int i)
{
	int		nb;

	nb = 0;
	if (size == 1)
	{
		d->tab[d->y][d->x++] = 'r';
		nb_in_buff(d, e->champs[d->champ].content[i]);
	}
	else if (size == 2)
	{
		if (!g_op_tab[d->op - 1].dir_size && (param == 128
				|| param == 32 || param == 8))
			d->tab[d->y][d->x++] = '%';
		nb_in_buff(d, (short)compute_param(e, d->champ, i, 2));
	}
	else if (size == 4)
	{
		d->tab[d->y][d->x++] = '%';
		nb_in_buff(d, compute_param(e, d->champ, i, 4));
	}
}

static int			check_params(t_env *e, t_decomp *d, t_ocp check)
{
	param_to_str(e, d, check.p1, check.s1, d->i + 1);
	if (check.p2)
	{
		d->tab[d->y][d->x++] = ',';
		param_to_str(e, d, check.p2, check.s2, d->i + 1 + check.s1);
	}
	if (check.p3)
	{
		d->tab[d->y][d->x++] = ',';
		param_to_str(e, d, check.p3, check.s3,
			d->i + 1 + check.s1 + check.s2);
	}
	return (1);
}

static void			init_tab(t_decomp *d)
{
	int		i;

	i = -1;
	while (++i < 300)
		ft_bzero(d->tab[i], COMMENT_LENGTH);
}

#include <stdio.h>
static void			print_tab(t_decomp *d)
{
	int		i;

	i = 0;
	while (*d->tab[i])
		printf("%s\n", d->tab[i++]);
}

int					decompile_champ(t_env *e, char *file_name, int champ)
{
	t_decomp	d;
	t_ocp		check;

	(void)file_name;
	(void)check;
	d.i = 0;
	d.champ = champ;
	init_tab(&d);
	put_header(e, &d);
	while (d.i < e->champs[champ].size)
	{
		d.x = 0;
		if (e->champs[champ].content[d.i] < 1 || e->champs[d.champ].content[d.i] > 16)
			return (0);
		d.op = e->champs[champ].content[d.i];
		nb_in_buff(&d, d.i);
		str_in_buff(&d, ":\t\t");
		str_in_buff(&d, g_op_tab[e->champs[champ].content[d.i] - 1].label);
		d.tab[d.y][d.x++] = ' ';
		d.i++;
		if (g_op_tab[e->champs[champ].content[d.i - 1] - 1].ocp)
		{
			check = check_ocp(e->champs[champ].content[d.i],
				g_op_tab[e->champs[champ].content[d.i - 1] - 1].dir_size);
			if (check.error == -1 ||
				!(check_params(e, &d, check)))
				return (0);
			d.i += 1 + check.s1 + check.s2 + check.s3;
		}
		else
			move_forward(e, &d);
		d.y++;
	}
	print_tab(&d);
	return (1);
}
