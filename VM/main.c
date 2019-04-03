/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/03 14:26:38 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


int usage(int ac, char **av)
{
	int i;
	int digit;
	int show;

	show = 0;
	i = 0;
	digit = 1;
	if (ac > 2)
	{
		while (av[2][i] && digit)
			digit = ft_isdigit(av[2][i++]);
		if (!ft_strcmp(av[1], "-dump"))
		{
			if (digit)
				ft_printf("There is dump\n");
			else
				show = 1;
		}
		else
		{
		}
	}
	if (show)
		ft_printf("./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...");
	ft_printf("{#6666ff}digit => %d, ac => %d, av[1] => %s\n", digit, ac, av[1]);
	return (0);
}

int main(int ac, char **av)
{
	usage(ac, av);	
	ft_printf("{#3333ff} (1) => USAGE!");
	return (0);
}
