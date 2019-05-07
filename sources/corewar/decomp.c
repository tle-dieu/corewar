/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:52:57 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/07 13:56:32 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int			free_buff_decomp(t_env *e)
{
	t_buff_d	*ptr;
	t_buff_d	*tmp;

	ptr = e->d.buff_d;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return (0);
}

static int			add_buff_link(t_env *e)
{
	t_buff_d	*new;
	t_buff_d	*ptr;
	int			i;

	i = -1;
	e->d.y = 0;
	if (!(new = (t_buff_d*)malloc(sizeof(t_buff_d))))
		return (0);
	new->next = NULL;
	while (++i <= 100)
		ft_bzero(new->tab[i], 100);
	if (!e->d.buff_d)
		e->d.buff_d = new;
	else
	{
		ptr = e->d.buff_d;
		while (ptr && ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (1);
}

static void			put_header(t_env *e, int champ)
{
	int		i;

	i = -1;
	e->d.buff_d = NULL;
	e->d.i = 0;
	e->d.champ = champ;
	add_buff_link(e);
	e->d.x = 0;
	str_in_buff(&e->d, ".name \"");
	str_in_buff(&e->d, e->champs[e->d.champ].name);
	str_in_buff(&e->d, "\"\n.comment \"");
	str_in_buff(&e->d, e->champs[e->d.champ].comment);
	e->d.y++;
	e->d.buff_d->tab[e->d.y][0] = '\n';
	e->d.y++;
}

static void			move_forward(t_env *e)
{
	e->d.buff_d->tab[e->d.y][e->d.x++] = '%';
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
		e->d.buff_d->tab[e->d.y][e->d.x++] = 'r';
		nb_in_buff(&e->d, e->champs[e->d.champ].content[i], 0);
	}
	else if (size == 2)
	{
		if (g_op_tab[e->d.op - 1].dir_size && (param == 128
					|| param == 32 || param == 8))
			e->d.buff_d->tab[e->d.y][e->d.x++] = '%';
		nb_in_buff(&e->d, (short)compute_param(e, e->d.champ, i, 2), 0);
	}
	else if (size == 4)
	{
		e->d.buff_d->tab[e->d.y][e->d.x++] = '%';
		nb_in_buff(&e->d, compute_param(e, e->d.champ, i, 4), 0);
	}
}

static int			check_params(t_env *e, t_ocp check)
{
	param_to_str(e, check.p[0], check.s[0], e->d.i + 1);
	if (check.p[1])
	{
		e->d.buff_d->tab[e->d.y][e->d.x++] = ',';
		param_to_str(e, check.p[1], check.s[1], e->d.i + 1 + check.s[0]);
	}
	if (check.p[2])
	{
		e->d.buff_d->tab[e->d.y][e->d.x++] = ',';
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
			return (free_buff_decomp(e));
		init_line(e);
		if (g_op_tab[e->champs[champ].content[e->d.i - 1] - 1].ocp)
		{
			check = check_ocp(e->champs[champ].content[e->d.i],
					g_op_tab[e->champs[champ].content[e->d.i - 1] - 1].dir_size,
					e->champs[champ].content[e->d.i - 1]);
			if (check.error == -1 || !(check_params(e, check)))
				return (free_buff_decomp(e));
			e->d.i += 1 + check.s[0] + check.s[1] + check.s[2];
		}
		else
			move_forward(e);
		if (e->d.y + 1 >= 100)
			add_buff_link(e);
		e->d.y++;
	}
	return (generate_decomp_file(&e->d, arg));
}
