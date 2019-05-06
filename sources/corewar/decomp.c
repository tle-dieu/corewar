/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 10:26:51 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/06 16:07:55 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

//Need: parsing avec check_champ, check_champ_size, check_magic_number,
//	split_champ, puis check_ocp et check_ocp_rights.

static void			put_header(t_env *e, int champ)
{
	int		i;

	i = -1;
	e->d.i = 0;
	e->d.champ = champ;
	while (++i < 300)
		ft_bzero(e->d.tab[i], COMMENT_LENGTH);
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
	param_to_str(e, check.p[0], check.s[0], e->d.i + 1);
	if (check.p[1])
	{
		e->d.tab[e->d.y][e->d.x++] = ',';
		param_to_str(e, check.p[1], check.s[1], e->d.i + 1 + check.s[0]);
	}
	if (check.p[2])
	{
		e->d.tab[e->d.y][e->d.x++] = ',';
		param_to_str(e, check.p[2], check.s[2],
			e->d.i + 1 + check.s[0] + check.s[1]);
	}
	return (1);
}

int					decompile_champ(t_env *e, char *arg, int champ)
{
	t_ocp	check;

	put_header(e, champ);
	while (e->d.i < e->champs[champ].size)
	{
		if (e->champs[champ].content[e->d.i] < 1
			|| e->champs[e->d.champ].content[e->d.i] > 16)
			return (0);
		init_line(e);
		if (g_op_tab[e->champs[champ].content[e->d.i - 1] - 1].ocp)
		{
			check = check_ocp(e->champs[champ].content[e->d.i],
				g_op_tab[e->champs[champ].content[e->d.i - 1] - 1].dir_size,
				e->champs[champ].content[e->d.i - 1]);
			if (check.error == -1 || !(check_params(e, check)))
				return (0);
			e->d.i += 1 + check.s[0] + check.s[1] + check.s[2];
		}
		else
			move_forward(e);
		if (e->d.y + 1 > 300)
			return (0);
		e->d.y++;
	}
	return (generate_decomp_file(&e->d, arg));
}
