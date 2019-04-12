/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:37:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/11 20:58:49 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void		aff(t_env *e, int *pc, t_proc *ptr)
{
	t_ocp	check;

	check = check_ocp(e->mem[(*pc + 1) % MEM_SIZE], 0);
	if (check.p1 == 64 && !check.p2 && !check.p3)
		ft_printf("%c", ptr->r[e->mem[*pc % MEM_SIZE]] % 256);
	*pc += 2;
}
