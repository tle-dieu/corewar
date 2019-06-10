/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_disass.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 22:45:28 by acompagn          #+#    #+#             */
/*   Updated: 2019/06/11 00:54:03 by tle-dieu         ###   ########.fr       */
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

static int	check_padding(unsigned char *line)
{
	unsigned padding_name;
	unsigned padding_comment;

	padding_name = *((unsigned *)(line + PROG_NAME_LENGTH + 4));
	padding_comment = *((unsigned *)(line + HEADER_SIZE - 4));
	return (!(padding_name || padding_comment));
}

int			check_champ_disass(t_env *e, t_disass *d)
{
	unsigned char	line[HEADER_SIZE];
	long			ret;

	ft_bzero(line, HEADER_SIZE);
	if ((ret = read(e->file->fd, line, HEADER_SIZE)) == -1)
		return (disass_error(e, NULL, d));
	if (swap_bytes4(*(unsigned *)line) != COREWAR_EXEC_MAGIC)
		return (disass_error(e, "Invalid magic", d));
	if (!check_padding(line))
		return (disass_error(e, "Wrong separators", d));
	d->size = swap_bytes4(*(unsigned *)(line + PROG_NAME_LENGTH + 8)); // fonctionne correctement mais a verifier
	if (!d->size || ret != HEADER_SIZE)
		return (disass_error(e, "Champion too small", d));
	return (split_champ(e, d, line));
}
