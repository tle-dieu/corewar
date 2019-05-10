/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_decomp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 22:45:28 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/10 16:03:54 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static void	check_ocp_rights(t_ocp *check, int inst, unsigned char ocp)
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

static void	find_param_size(t_ocp *check, int ocp, int on_two)
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

t_ocp		check_ocp(int ocp, int on_two, int inst)
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

static int	split_champ(t_env *e, t_decomp *d, unsigned char *line, int ret)
{
	int		i;
	int		k;

	i = 3;
	k = 0;
	d->size = line[PROG_NAME_LENGTH + 10] * 256
		+ line[PROG_NAME_LENGTH + 11];
	if (d->size <= 0)
	{
		ft_printf("Champion too small: %d\n", d->size);
		return (0);
	}
	if (!(d->content = (unsigned char *)malloc(sizeof(unsigned char)
			* (d->size + 2))))
		alloc_error(e);
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
	if ((ret = read(e->file->fd, d->content, d->size + 1)) == -1 || ret != d->size)
	{
		if (ret == -1)
			ft_printf("error: %s\n", strerror(errno));
		else
			ft_printf("Read size %d different from announced size %d\n", ret, d->size);
		return (free_buff_decomp(d));
	}
	return (!d->content[d->size]);
}

static int	check_padding(unsigned char *line)
{
	int		i;
	int		k;

	i = PROG_NAME_LENGTH + 3;
	k = 4;
	while (1)
	{
		k = i < COMMENT_LENGTH ? 6 : 4;
		while (k--)
		{
			if (line[i++])
			{
				ft_printf("wrong separators\n");
				return (0);
			}
		}
		if (i > COMMENT_LENGTH)
			break ;
		i += COMMENT_LENGTH + 3;
	}
	return (1);
}

int			check_champ_decomp(t_env *e, t_decomp *d)
{
	unsigned char	line[NAME_COMM_SIZE + 16];
	int				ret;
	int				b;
	int				i;

	b = 16;
	i = 0;
	ft_bzero(line, NAME_COMM_SIZE + 16);
	if ((ret = read(e->file->fd, line, NAME_COMM_SIZE + 16) == -1) || line[0])
	{
		if (ret == -1)
			ft_printf("error: %s\n", strerror(errno));
		else
			ft_printf("Invalid magic\n");
		return (free_buff_decomp(d));
	}
	while (b >= 0)
	{
		if (line[++i] != (COREWAR_EXEC_MAGIC >> b & 0xff))
		{
			ft_printf("Invalid magic\n");
			return (0);
		}
		b -= 8;
	}
	if (!check_padding(line))
		return (0);
	return (split_champ(e, d, line, ret));
}
