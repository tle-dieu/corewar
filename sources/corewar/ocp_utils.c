/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ocp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 11:01:09 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/29 11:01:47 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void     check_ocp_rights(t_ocp *check, int inst, unsigned char ocp)
{
	int i;

	i = -1;
	while (++i < 4)
	{
		if (i < g_op_tab[inst - 1].nb_param)
		{
			if (!((g_op_tab[inst - 1].param[i] >> (((ocp >> ((3 - i) * 2)) & 3) - 1)) & 1))
				check->error = 1;
		}
		else if ((ocp >> ((3 - i) * 2)) & 3)
			check->error = 1;
	}
}

static void		find_param_size(t_ocp *check, int ocp, int on_two)
{
	if (ocp >= 192 || ocp < 64)
		check->s1 = (ocp >= 192) ? 2 : 0;
	else
	{
		if (ocp >= 128)
			check->s1 = on_two ? 2 : 4;
		else
			check->s1 = 1;
	}
	if (ocp - check->p1 >= 48 || ocp - check->p1 < 16)
		check->s2 = (ocp - check->p1 >= 48) ? 2 : 0;
	else
	{
		if (ocp - check->p1 >= 32)
			check->s2 = on_two ? 2 : 4;
		else
			check->s2 = 1;
	}
	if (check->p3 == 12 || check->p3 == 4)
		check->s3 = (check->p3 == 12) ? 2 : 1;
	else if (check->p3 == 8)
		check->s3 = on_two ? 2 : 4;
}


t_ocp			check_ocp(int ocp, int on_two, int inst)
{
	t_ocp	check;

	check = (t_ocp){0, 0, 0, 0, 0, 0, 0};
	check.error = ocp < 64;
	if (ocp >= 192 || ocp < 64)
		check.p1 = (ocp >= 192) ? 192 : 0;
	else
		check.p1 = (ocp >= 128) ? 128 : 64;
	if (ocp - check.p1 >= 48 || ocp - check.p1 < 16)
		check.p2 = (ocp - check.p1 >= 48) ? 48 : 0;
	else
		check.p2 = (ocp - check.p1 >= 32) ? 32 : 16;
	check.p3 = ocp - check.p1 - check.p2;
	find_param_size(&check, ocp, on_two);
	check_ocp_rights(&check, inst, ocp);
	return (check);
}
