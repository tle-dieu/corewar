/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 14:48:55 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/05 12:34:49 by acompagn         ###   ########.fr       */
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

void		live(t_env *e, int *i)
{
	printf("in live :: ");
	*i += 5;
	printf("OK");
	(void)e;
}

void		ld(t_env *e, int *i)
{
	t_ocp	check;

	printf("in ld :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || check.param2 != 16 || check.param3)
	{
		if (!check.param1)
			printf("ERROR:: no parameter 1\n");
		if (check.param2 != 16)
			printf("ERROR:: parameter 2 must be a register\n");
		if (check.param3)
			printf("ERROR:: only 2 parameters accepted\n");
	}
	else
	{
		printf("OK");
		if (check.param1 == 128)
			*i += 7;
		if (check.param1 == 192)
			*i += 5;
		if (check.param1 == 64)
			*i += 4;
	}
}

void		st(t_env *e, int *i)
{
	t_ocp	check;
	
	printf("in st :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 || check.param1 != 64)
	{
		if (!check.param1 || !check.param2 || check.param3)
			printf("ERROR :: wrong args\n");
		if (check.param1 != 64)
			printf("ERROR :: parameter 1 must be a register\n");
	}
	else
	{
		printf("OK");
		*i += 3;
		if (check.param2 == 32)
			*i += 4;
		if (check.param2 == 16)
			*i += 1;
		if (check.param2 == 48)
			*i += 2;
	}
}

void		add(t_env *e, int *i)
{
	t_ocp	check;
	
	printf("in add :: ");
	check = check_ocp(e->line[*i + 1]);
	if (check.param1 != 64 || check.param2 != 16 || check.param3 != 4)
		printf("ERROR :: wrong args, must have 3 parameters\n");
	else
	{
		*i += 5;
		printf("OK");
	}
}

void		sub(t_env *e, int *i)
{
	t_ocp	check;

	printf("in sub :: ");
	check = check_ocp(e->line[*i + 1]);
	if (check.param1 != 64 || check.param2 != 16 || check.param3 != 4)
		printf("ERROR :: wrong args, must have 3 parameters\n");
	else
	{
		*i += 5;
		printf("OK");
	}
}

void		and(t_env *e, int *i)
{
	t_ocp	check;

	printf("in and :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 != 4)
		printf("ERROR :: wrong args, must have 3, last = register\n");
	else
	{
		printf("OK");
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

	printf("in or :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 != 4)
		printf("ERROR :: wrong args, must have 3, last = register\n");
	else
	{
		printf("OK");
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

	printf("in xor :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || check.param3 != 4)
		printf("ERROR :: wrong args, must have 3, last = register\n");
	else
	{
		printf("OK");
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
	printf("in zjmp :: ");
	*i += 3;
	(void)e;
	printf("OK");
}

void		ldi(t_env *e, int *i)
{
	t_ocp	check;

	printf("in ldi :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || !check.param3)
		printf("ERROR :: Must have 3 parameters\n");
	else
	{
		printf("OK");
		*i = (check.param1 == 128 || check.param1 == 192) ? *i + 2 : *i + 1;
		*i = (check.param2 == 32 || check.param2 == 48) ? *i + 3 : *i + 2;
	   	*i = (check.param3 == 8 || check.param3 == 12) ? *i + 3 : *i + 2;
	}
}

void		sti(t_env *e, int *i)
{
	t_ocp	check;
	
	printf("in sti :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || !check.param3)
		printf("ERROR :: Must have 3 parameters\n");
	else
	{
		printf("OK");
		*i = (check.param1 == 192 || check.param1 == 128) ? *i + 2 : *i + 1;
		*i = (check.param2 == 32 || check.param2 == 48) ? *i + 3 : *i + 2;
	   	*i = (check.param3 == 8 || check.param3 == 12) ? *i + 3 : *i + 2;
	}
}

void		op_fork(t_env *e, int *i)
{
	printf("in fork :: ");
	*i += 3;
	printf("OK");
	(void)e;
}

void		lld(t_env *e, int *i)
{
	t_ocp	check;

	printf("in lld :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || check.param2 != 16 || check.param3)
	{
		if (!check.param1)
			printf("ERROR:: no parameter 1\n");
		if (check.param2 != 16)
			printf("ERROR:: parameter 2 must be a register\n");
		if (check.param3)
			printf("ERROR:: only 2 parameters accepted\n");
	}
	else
	{
		printf("OK");
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

	printf("in lldi :: ");
	check = check_ocp(e->line[*i + 1]);
	if (!check.param1 || !check.param2 || !check.param3)
		printf("ERROR :: Must have 3 parameters\n");
	else
	{
		printf("OK");
		*i = (check.param1 == 128 || check.param1 == 192) ? *i + 2 : *i + 1;
		*i = (check.param2 == 32 || check.param2 == 48) ? *i + 3 : *i + 2;
	   	*i = (check.param3 == 8 || check.param3 == 12) ? *i + 3 : *i + 2;
	}

	(void)e;
	(void)i;
}

void		lfork(t_env *e, int *i)
{
	printf("in lfork :: ");
	*i += 3;
	printf("OK");
	(void)e;
}

void		aff(t_env *e, int *i)
{
	t_ocp	check;

	printf("in aff :: ");
	check = check_ocp(e->line[*i + 1]);
	if (check.param1 != 64 || check.param2 || check.param3)
		printf("ERROR :: wrong args, must have one register\n");
	else
	{
		printf("OK");
		*i += 2;
	}
}
