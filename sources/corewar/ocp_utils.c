/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ocp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 11:01:09 by acompagn          #+#    #+#             */
/*   Updated: 2019/06/21 11:52:54 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void	check_ocp_rights(t_ocp *check, int inst, unsigned char ocp)
{
	int i;
	int	shift;

	i = -1;
	while (++i < 4)
	{
		if (i < g_op_tab[inst - 1].nb_param)
		{
			shift = ((ocp >> ((3 - i) * 2)) & 3);
			if (!shift || !((g_op_tab[inst - 1].param[i] >> (shift - 1)) & 1))
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
