/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 10:26:51 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/24 13:25:54 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void			put_header(t_env *e)
{
	e->d.y = 0;
	e->d.x = 0;
	str_in_buff(&e->d, ".name \"");
	str_in_buff(&e->d, e->champs[e->d.champ].name);
	str_in_buff(&e->d, "\"\n.comment \"");
	str_in_buff(&e->d, e->champs[e->d.champ].comment);
	e->d.y++;
	e->d.tab[e->d.y][0] = '\n';
	e->d.y++;
}

static void			move_forward(t_env *e)
{
	e->d.tab[e->d.y][e->d.x++] = '%';
	if (e->d.op == 1)
	{
		nb_in_buff(&e->d, compute_param(e, e->d.champ, e->d.i, 4), 0);
		e->d.i += 4;
	}
	else if (e->d.op == 9 || e->d.op == 15 || e->d.op == 12)
	{
		nb_in_buff(&e->d, (short)compute_param(e, e->d.champ, e->d.i, 2), 0);
		e->d.i += 2;
	}
}

static void			param_to_str(t_env *e, int param, int size, int i)
{
	int		nb;

	nb = 0;
	if (size == 1)
	{
		e->d.tab[e->d.y][e->d.x++] = 'r';
		nb_in_buff(&e->d, e->champs[e->d.champ].content[i], 0);
	}
	else if (size == 2)
	{
		if (g_op_tab[e->d.op - 1].dir_size && (param == 128
				|| param == 32 || param == 8))
			e->d.tab[e->d.y][e->d.x++] = '%';
		nb_in_buff(&e->d, (short)compute_param(e, e->d.champ, i, 2), 0);
	}
	else if (size == 4)
	{
		e->d.tab[e->d.y][e->d.x++] = '%';
		nb_in_buff(&e->d, compute_param(e, e->d.champ, i, 4), 0);
	}
}

static int			check_params(t_env *e, t_ocp check)
{
	param_to_str(e, check.p1, check.s1, e->d.i + 1);
	if (check.p2)
	{
		e->d.tab[e->d.y][e->d.x++] = ',';
		param_to_str(e, check.p2, check.s2, e->d.i + 1 + check.s1);
	}
	if (check.p3)
	{
		e->d.tab[e->d.y][e->d.x++] = ',';
		param_to_str(e, check.p3, check.s3,
			e->d.i + 1 + check.s1 + check.s2);
	}
	return (1);
}

int					decompile_champ(t_env *e, int champ)
{
	t_ocp	check;

	init_decomp(&e->d, champ);
	put_header(e);
	while (e->d.i < e->champs[champ].size)
	{
		if (e->champs[champ].content[e->d.i] < 1
			|| e->champs[e->d.champ].content[e->d.i] > 16)
			return (0);
		init_line(e);
		if (g_op_tab[e->champs[champ].content[e->d.i - 1] - 1].ocp)
		{
			check = check_ocp(e->champs[champ].content[e->d.i],
				g_op_tab[e->champs[champ].content[e->d.i - 1] - 1].dir_size);
			if (check.error == -1 || !(check_params(e, check)))
				return (0);
			e->d.i += 1 + check.s1 + check.s2 + check.s3;
		}
		else
			move_forward(e);
		e->d.y++;
	}
	print_tab(&e->d);
	return (1);
}
