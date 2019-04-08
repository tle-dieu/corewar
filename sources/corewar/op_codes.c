/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_codes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:14:09 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/08 14:16:17 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_ocp		check_ocp(int ocp)
{
	t_ocp	check;

	if (ocp >= 192 || ocp < 64)
		check.param1 = (ocp >= 192) ? 192 : 0;
	else
		check.param1 = (ocp >= 128) ? 128 : 64;
	if (ocp - check.param1 >= 48 || ocp - check.param1 < 16)
		check.param2 = (ocp - check.param1 >= 48) ? 48 : 0;
	else
		check.param2 = (ocp - check.param1 >= 32) ? 32 : 16;
	check.param3 = ocp - check.param1 - check.param2;
	return (check);
}

void		live(t_env *e, int *i, t_proc *ptr)
{
	int		player_nb;

	if (ptr)
	{
		player_nb = e->mem[
	}
	*i += 5;
}

void		ld(t_env *e, int *i, t_proc *ptr)
{
	t_ocp	check;
	int		tmp;

	check = check_ocp(e->line[*i + 1]);
	tmp = *i + 1;
	if (!check.param1 || check.param2 != 16 || check.param3)
			ft_printf("ERROR::\n");
	else
	{
		if (check.param1 == 128)
			*i += 7;
		else if (check.param1 == 192)
			*i += 5;
		else if (check.param1 == 64)
			*i += 4;
		if (ptr && *i - 1 > 0 && *i - 1 < 16)
			ptr->r[e->mem[*i - 1]] = e->mem[tmp];
	}
}

void		st(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in st :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 || check.param1 != 64)
	{
		if (!check.param1 || !check.param2 || check.param3)
			ft_printf("ERROR :: wrong args\n");
		if (check.param1 != 64)
			ft_printf("ERROR :: parameter 1 must be a register\n");
	}
	else
	{
		ft_printf("OK");
		*i += 3;
		if (check.param2 == 32)
			*i += 4;
		else if (check.param2 == 16)
			*i += 1;
		else if (check.param2 == 48)
			*i += 2;
	}
}

void		add(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in add :: ");
	check = check_ocp(e->line[*i + 1]);
	if (check.param1 != 64 || check.param2 != 16 || check.param3 != 4)
		ft_printf("ERROR :: wrong args, must have 3 parameters\n");
	else
	{
		*i += 5;
		ft_printf("OK");
	}
}

void		sub(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in sub :: ");
	check = check_ocp(e->line[*i + 1]);
	if (check.param1 != 64 || check.param2 != 16 || check.param3 != 4)
		ft_printf("ERROR :: wrong args, must have 3 parameters\n");
	else
	{
		*i += 5;
		ft_printf("OK");
	}
}

void		and(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in and :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 != 4)
		ft_printf("ERROR :: wrong args, must have 3, last = register\n");
	else
	{
		ft_printf("OK");
		*i += 3;
		if (check.param1 == 128)
			*i += 4;
		else
			*i = (check.param1 == 192) ? *i + 2 : *i + 1;
		if (check.param2 == 32)
			*i += 4;
		else
			*i = (check.param2 == 48) ? *i + 2 : *i + 1;
	}
}

void		or(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in or :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 != 4)
		ft_printf("ERROR :: wrong args, must have 3, last = register\n");
	else
	{
		ft_printf("OK");
		*i += 3;
		if (check.param1 == 128)
			*i += 4;
		else
			*i = (check.param1 == 192) ? *i + 2 : *i + 1;
		if (check.param2 == 32)
			*i += 4;
		else
			*i = (check.param2 == 48) ? *i + 2 : *i + 1;
	}
}

void		xor(t_env *e, int *i)
{
	t_ocp	check;

	ft_printf("in xor :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 != 4)
		ft_printf("ERROR :: wrong args, must have 3, last = register\n");
	else
	{
		ft_printf("OK");
		*i += 3;
		if (check.param1 == 128)
			*i += 4;
		else
			*i = (check.param1 == 192) ? *i + 2 : *i + 1;
		if (check.param2 == 32)
			*i += 4;
		else
			*i = (check.param2 == 48) ? *i + 2 : *i + 1;
	}
}

void		zjmp(t_env *e, int *i)
{
	ft_printf("in zjmp :: ");
	*i += 3;
	(void)e;
	ft_printf("OK");
}
