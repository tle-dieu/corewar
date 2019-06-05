/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disass.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:52:57 by acompagn          #+#    #+#             */
/*   Updated: 2019/06/05 17:09:31 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static int			put_header(t_env *e, t_disass *d)
{
	int		i;

	i = -1;
	d->buff_d = NULL;
	d->main_ptr = NULL;
	d->content = NULL;
	d->i = 0;
	if (!check_champ_disass(e, d) || !add_buff_link(e, d))
		return (free_buff_disass(d));
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

static void			move_forward(t_disass *d)
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

static int			param_to_str(t_disass *d, int param, int size, int i)
{
	if (size == 1)
	{
		d->buff_d->tab[d->y][d->x++] = 'r';
		if (d->content[i] > 0 && d->content[i] < 17)
			nb_in_buff(d, d->content[i], 0);
		else
			return (0);
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
	return (1);
}

static int			check_disass_params(t_env *e, t_disass *d, t_ocp check)
{
	if (!param_to_str(d, check.p[0], check.s[0], d->i + 1))
	{
		return (disass_error(e,
				"Wrong register, must be between 1 and 16", d));
	}
	if (check.p[1])
	{
		d->buff_d->tab[d->y][d->x++] = ',';
		if (!param_to_str(d, check.p[1], check.s[1], d->i + 1 + check.s[0]))
			return (disass_error(e,
					"Wrong register, must be between 1 and 16", d));
	}
	if (check.p[2])
	{
		d->buff_d->tab[d->y][d->x++] = ',';
		if (!param_to_str(d, check.p[2], check.s[2],
				d->i + 1 + check.s[0] + check.s[1]))
			return (disass_error(e,
					"Wrong register, must be between 1 and 16", d));
	}
	return (1);
}

int					disassemble_champ(t_env *e)
{
	t_ocp		check;
	t_disass	d;

	if (!put_header(e, &d))
		return (0);
	while (d.i < d.size)
	{
		if (d.content[d.i] < 1 || d.content[d.i] > 16)
			return (disass_error(e, "Wrong instruction", &d));
		init_line(&d);
		if (g_op_tab[d.content[d.i - 1] - 1].ocp)
		{
			check = check_ocp(d.content[d.i],
					g_op_tab[d.content[d.i - 1] - 1].dir_size,
					d.content[d.i - 1]);
			if (check.error == -1 || !(check_disass_params(e, &d, check)))
				return (check.error == -1 ? free_buff_disass(&d) : 0);
			d.i += 1 + check.s[0] + check.s[1] + check.s[2];
		}
		else
			move_forward(&d);
		d.y++;
		d.y == BS_DECOMP - 1 ? add_buff_link(e, &d) : 1;
	}
	return (generate_disass_file(e, &d, d.main_ptr));
}
