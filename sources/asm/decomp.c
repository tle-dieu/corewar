/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:52:57 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/09 19:57:49 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void			check_ocp_rights(t_ocp *check, int inst, unsigned char ocp)
{
	int i;

	i = -1;
	while (++i < 4)
	{
		if (i < g_op_tab[inst - 1].nb_param)
		{
			if (!((g_op_tab[inst - 1].param[i]
							>> (((ocp >> ((3 - i) * 2)) & 3) - 1)) & 1))
				check->error = 1;
		}
		else if ((ocp >> ((3 - i) * 2)) & 3)
			check->error = 1;
	}
}

static void			find_param_size(t_ocp *check, int ocp, int on_two)
{
	if (ocp >= 192 || ocp < 64)
		check->s[0] = (ocp >= 192) ? 2 : 0;
	else
	{
		if (ocp >= 128)
			check->s[0] = on_two ? 2 : 4;
		else
			check->s[0] = 1;
	}
	if (ocp - check->p[0] >= 48 || ocp - check->p[0] < 16)
		check->s[1] = (ocp - check->p[0] >= 48) ? 2 : 0;
	else
	{
		if (ocp - check->p[0] >= 32)
			check->s[1] = on_two ? 2 : 4;
		else
			check->s[1] = 1;
	}
	if (check->p[2] == 12 || check->p[2] == 4)
		check->s[2] = (check->p[2] == 12) ? 2 : 1;
	else if (check->p[2] == 8)
		check->s[2] = on_two ? 2 : 4;
}

static t_ocp		check_ocp(int ocp, int on_two, int inst)
{
	t_ocp	check;
	int		i;

	i = -1;
	while (++i < 3)
	{
		check.v[i] = 0;
		check.p[i] = 0;
		check.s[i] = 0;
	}
	check.shift = 64;
	check.error = ocp < 64;
	if (ocp >= 192 || ocp < 64)
		check.p[0] = (ocp >= 192) ? 192 : 0;
	else
		check.p[0] = (ocp >= 128) ? 128 : 64;
	if (ocp - check.p[0] >= 48 || ocp - check.p[0] < 16)
		check.p[1] = (ocp - check.p[0] >= 48) ? 48 : 0;
	else
		check.p[1] = (ocp - check.p[0] >= 32) ? 32 : 16;
	check.p[2] = ocp - check.p[0] - check.p[1];
	find_param_size(&check, ocp, on_two);
	check_ocp_rights(&check, inst, ocp);
	return (check);
}

int					free_buff_decomp(t_decomp *d)
{
	t_buff_d	*ptr;
	t_buff_d	*tmp;

	ptr = d->buff_d;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return (0);
}

static int			add_buff_link(t_decomp *d)
{
	t_buff_d	*new;
	t_buff_d	*ptr;
	int			i;

	i = -1;
	ft_printf("add link\n");
	d->y = 0;
	if (!(new = (t_buff_d*)malloc(sizeof(t_buff_d))))
		return (0);
	new->next = NULL;
	while (++i <= 100)
		ft_bzero(new->tab[i], COMMENT_LENGTH + 11);
	if (!d->buff_d)
	{
		new->id = 1;
		d->buff_d = new;
		d->main_ptr = new;
	}
	else
	{
		ptr = d->buff_d;
		while (ptr && ptr->next)
			ptr = ptr->next;
		new->id = ptr->id + 1;
		ptr->next = new;
		d->buff_d = ptr->next;
	}
	return (1);
}

static int			check_champ_decomp(t_decomp *d, t_file *file)
{
	unsigned char	line[NAME_COMM_SIZE + 16];
	int				b;
	int				i;
	int				k;
	int				ret;

	b = 16;
	i = 0;
	k = 0;
	ft_bzero(line, NAME_COMM_SIZE + 16);
	if (file->fd == -1 || !file->fd)
		return (0);
	if ((ret = read(file->fd, line, NAME_COMM_SIZE + 16) == -1))
		return (0);
	while (b >= 0)
	{
		if (line[++i] != (COREWAR_EXEC_MAGIC >> b & 0xff))
			return (0);
		b -= 8;
	}
	d->size = line[PROG_NAME_LENGTH + 10] * 256 + line[PROG_NAME_LENGTH + 11];
	if (!(d->content = (unsigned char *)malloc(sizeof(unsigned char) * (d->size + 2))))
		return (0);
	ft_bzero(d->content, d->size + 2);
	while (i++ < NAME_COMM_SIZE + 8)
	{
		if (i == PROG_NAME_LENGTH + 8)
			k = 0;
		if (i < PROG_NAME_LENGTH + 8)
			d->name[k++] = line[i];
		else if (i > PROG_NAME_LENGTH + 11)
			d->comment[k++] = line[i];
	}
	if ((ret = read(file->fd, d->content, d->size + 1)) == -1)
		return (0);
	if (d->content[d->size])
	{
		free(d->content);
		return (0);
	}
	return (1);
}

static int			put_header(t_decomp *d, t_file *file)
{
	int		i;

	i = -1;
	d->buff_d = NULL;
	d->i = 0;
	if (!check_champ_decomp(d, file) || !add_buff_link(d))
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

int					decompile_champ(t_file *file)
{
	t_ocp		check;
	t_decomp	d;

	if (!put_header(&d, file))
		return (0);
	ft_printf("size = %d\n", d.size);
	while (d.i < d.size)
	{
		if (d.content[d.i] < 1 || d.content[d.i] > 16)
			return (free_buff_decomp(&d));
		init_line(&d);
		if (g_op_tab[d.content[d.i - 1] - 1].ocp)
		{
			check = check_ocp(d.content[d.i],
					g_op_tab[d.content[d.i - 1] - 1].dir_size, d.content[d.i - 1]);
			if (check.error == -1 || !(check_decomp_params(&d, check)))
				return (free_buff_decomp(&d));
			d.i += 1 + check.s[0] + check.s[1] + check.s[2];
		}
		else
			move_forward(&d);
		d.y++;
		if (d.y >= 100)
			add_buff_link(&d);
	}
	return (generate_decomp_file(&d, file));
}
