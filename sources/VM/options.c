/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:13:16 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/03 17:14:12 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	strisdigit(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_champ_params(int ac, char **av, int elem, int *show)
{
	int		champ_nb;
	char	*tmp;

	champ_nb = 0;
	while (elem != ac && !*show)
	{
		if (!ft_strcmp(av[elem], "-n"))
		{
			if (strisdigit(av[elem + 1]) && ac > elem + 2)
				elem += 2;
			else
				*show = 1;
		}
		if ((tmp = ft_strrchr(av[elem], '.')) && !ft_strcmp(tmp, ".cor"))
		{
			++elem;
			champ_nb++;
		}
		else
			*show = 1;
	}
	return (champ_nb);
}

int	usage(int ac, char **av)
{
	int show;
	int elem;
	int champ_nb;

	champ_nb = 0;
	elem = 1;
	show = 1;
	if (ac > 2)
	{
		show = 0;
		if (!ft_strcmp(av[1], "-dump"))
		{
			if (strisdigit(av[2]) && ac > 4)
				elem = 3;
			else
				show = 1;
		}
		champ_nb = check_champ_params(ac, av, elem, &show);
	}
	if (show || champ_nb < 2 || champ_nb > 4)
		ft_printf("usage:\n	%s [-dump nbr_cycles] [[-n number] champion1.cor] ...\n	You must enter between 2 and 4 champions\n	Champions must have the .cor extension\n\n", av[0]);
	return (0);
}
