/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 18:22:48 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/28 20:53:01 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include "libft.h"

int		check_ocp(int inst, unsigned char ocp)
{
	int i;
	int error;

	ft_printf("inst: %s\n", g_op_tab[inst - 1].label);
	i = -1;
	ft_printf("ocp %08b\n", ocp);
	while (++i < 4)
	{
		ft_printf("param%d: %08b >> ocp_decale: %08b - 1 == ", i + 1, g_op_tab[inst - 1].param[i], (ocp >> ((3 - i) * 2)) & 3);
		ft_printf("%08b\n", g_op_tab[inst - 1].param[i] >> (((ocp >> ((3 - i) * 2)) & 3) - 1));
		if (i < g_op_tab[inst - 1].nb_param)
		{
			if (!((g_op_tab[inst - 1].param[i] >> (((ocp >> ((3 - i) * 2)) & 3) - 1)) & 1))
			{
				ft_printf("{#ff3333}error param%d\n{R}", i + 1);
				error = 1;
			}
		}
		else if ((ocp >> ((3 - i) * 2)) & 3)
		{
			error = 1;
			ft_printf("{#ff3333}too many param\n{R}");
		}
	}
	if (!error)
		ft_printf("{rgb(12,231,58)}params ok\n{R}");
	return (1);
}

int		main(void)
{
	check_ocp(1, 0b10000000);
}

	/* {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG} */
