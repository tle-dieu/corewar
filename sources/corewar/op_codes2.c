/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_codes2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:15:29 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/08 14:15:52 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		ldi(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in ldi :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || !check.param3)
		ft_printf("ERROR :: Must have 3 parameters\n");
	else
	{
		ft_printf("OK");
		*i = (check.param1 == 128 || check.param1 == 192) ? *i + 2 : *i + 1;
		*i = (check.param2 == 32 || check.param2 == 48) ? *i + 3 : *i + 2;
		*i = (check.param3 == 8 || check.param3 == 12) ? *i + 3 : *i + 2;
	}
}

void		sti(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in sti :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || !check.param3)
		ft_printf("ERROR :: Must have 3 parameters\n");
	else
	{
		ft_printf("OK");
		*i = (check.param1 == 192 || check.param1 == 128) ? *i + 2 : *i + 1;
		*i = (check.param2 == 32 || check.param2 == 48) ? *i + 3 : *i + 2;
		*i = (check.param3 == 8 || check.param3 == 12) ? *i + 3 : *i + 2;
	}
}

void		op_fork(t_env *e, int *i)
{
	ft_printf("in fork :: ");
	*i += 3;
	ft_printf("OK");
	(void)e;
}

void		lld(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in lld :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || check.param2 != 16 || check.param3)
	{
		if (!check.param1)
			ft_printf("ERROR:: no parameter 1\n");
		if (check.param2 != 16)
			ft_printf("ERROR:: parameter 2 must be a register\n");
		if (check.param3)
			ft_printf("ERROR:: only 2 parameters accepted\n");
	}
	else
	{
		ft_printf("OK");
		if (check.param1 == 128)
			*i += 7;
		if (check.param1 == 192)
			*i += 5;
		if (check.param1 == 64)
			*i += 4;
	}
}

void		lldi(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in lldi :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || !check.param3)
		ft_printf("ERROR :: Must have 3 parameters\n");
	else
	{
		ft_printf("OK");
		*i = (check.param1 == 128 || check.param1 == 192) ? *i + 2 : *i + 1;
		*i = (check.param2 == 32 || check.param2 == 48) ? *i + 3 : *i + 2;
		*i = (check.param3 == 8 || check.param3 == 12) ? *i + 3 : *i + 2;
	}
}

void		lfork(t_env *e, int *i)
{
	ft_printf("in lfork :: ");
	*i += 3;
	ft_printf("OK");
	(void)e;
}

void		aff(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in aff :: ");
	check = check_ocp(e->line[*i + 1]);
	if (check.param1 != 64 || check.param2 || check.param3)
		ft_printf("ERROR :: wrong args, must have one register\n");
	else
	{
		ft_printf("OK");
		*i += 2;
	}
}
