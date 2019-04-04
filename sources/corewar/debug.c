/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:33 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/04 16:31:14 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "corewar.h"
#include "op.h"

void	print_env(t_env env)
{
	int i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (!(i % 64))
			ft_printf("\n");
		if ((int)env.mem[i] == 0)
			ft_printf("{#666666}%02x ", (int)env.mem[i]);
		else 
			ft_printf("{reset}%02x ", (int)env.mem[i]);
		i++;
	}
	ft_printf("{reset}\ntotal cycles: %-10d cyles to die: %d / %-10d \n", env.c_total, env.c, env.c_to_die);
}
