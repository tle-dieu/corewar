/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/04 14:43:38 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "corewar.h"
#include "op.h"
#include "libft.h"

int main(int ac, char **av)
{
	t_env env;
	unsigned char mem[MEM_SIZE];

	env = (t_env){0, 1536, 0, NULL, mem};
	if (usage(ac, av))
		exit(1);
	ft_bzero(env.mem, MEM_SIZE);
	print_env(env);
	return (0);
}
