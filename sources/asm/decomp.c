/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:52:57 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/10 03:59:38 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static int			put_header(t_env *e, t_decomp *d)
{
	int		i;

	i = -1;
	d->buff_d = NULL;
	d->content = NULL;
	d->i = 0;
	if (!check_champ_decomp(e, d) || !add_buff_link(e, d))
		return (free_buff_decomp(d));
	i = -1;
	d->x = 0;
	str_in_buff(d, ".name \"");
	str_in_buff(d, d->name);
	str_in_buff(d, "\"\n.comment \"");
	str_in_buff(d, d->comment);
	str_in_buff(d, "\"");
	d->y++;
	d->buff_d->tab[d->y][0] = '\n';
	d->y++;
	return (1);
}

static void			move_forward(t_decomp *d)
{
	d->buff_d->tab[d->y][d->x++] = '%';
	if (d->op == 1)
	{
		nb_in_buff(d, compute_param(d, d->i, 4), 0);
		d->i += 4;
	}
	else if (d->op == 9 || d->op == 15 || d->op == 12)
	{
		nb_in_buff(d, (short)compute_param(d, d->i, 2), 0);
		d->i += 2;
	}
}

static void			param_to_str(t_decomp *d, int param, int size, int i)
{
	if (size == 1)
	{
		d->buff_d->tab[d->y][d->x++] = 'r';
		nb_in_buff(d, d->content[i], 0);
	}
	else if (size == 2)
	{
		if (g_op_tab[d->op - 1].dir_size && (param == 128
					|| param == 32 || param == 8))
			d->buff_d->tab[d->y][d->x++] = '%';
		nb_in_buff(d, (short)compute_param(d, i, 2), 0);
	}
	else if (size == 4)
	{
		d->buff_d->tab[d->y][d->x++] = '%';
		nb_in_buff(d, compute_param(d, i, 4), 0);
	}
}

static int			check_decomp_params(t_decomp *d, t_ocp check)
{
	param_to_str(d, check.p[0], check.s[0], d->i + 1);
	if (check.p[1])
	{
		d->buff_d->tab[d->y][d->x++] = ',';
		param_to_str(d, check.p[1], check.s[1], d->i + 1 + check.s[0]);
	}
	if (check.p[2])
	{
		d->buff_d->tab[d->y][d->x++] = ',';
		param_to_str(d, check.p[2], check.s[2],
				d->i + 1 + check.s[0] + check.s[1]);
	}
	return (1);
}

int					decompile_champ(t_env *e)
{
	t_ocp		check;
	t_decomp	d;

	if (!put_header(e, &d))
		return (0);
	while (d.i < d.size)
	{
		if (d.content[d.i] < 1 || d.content[d.i] > 16)
			return (free_buff_decomp(&d));
		init_line(&d);
		if (g_op_tab[d.content[d.i - 1] - 1].ocp)
		{
			check = check_ocp(d.content[d.i],
					g_op_tab[d.content[d.i - 1] - 1].dir_size,
					d.content[d.i - 1]);
			if (check.error == -1 || !(check_decomp_params(&d, check)))
				return (free_buff_decomp(&d));
			d.i += 1 + check.s[0] + check.s[1] + check.s[2];
		}
		else
			move_forward(&d);
		d.y++;
		d.y == BS_DECOMP - 1 ? add_buff_link(e, &d) : 1;
	}
	return (generate_decomp_file(e, &d, d.main_ptr));
}
