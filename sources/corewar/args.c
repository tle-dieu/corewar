/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:04 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/11 20:57:54 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int		strisdigit(char *str)
{
	int i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int		check_params(t_env *e, int ac, char **av, int elem, int *show)
{
	char	*tmp;

	while (elem != ac && !*show)
	{
		if (!ft_strcmp(av[elem], "-n"))
		{
			if (strisdigit(av[elem + 1]) && ac > elem + 2)
			{
				e->champs[e->nb_champ].chosen_id[0] = 1;
				e->champs[e->nb_champ].chosen_id[1] = ft_atoi(av[elem + 1]);
				elem += 2;
			}
			else
				*show = 1;
		}
		if ((tmp = ft_strrchr(av[elem], '.')) && !ft_strcmp(tmp, ".cor"))
			e->champs[e->nb_champ++].file = elem++;
		else
			*show = 1;
	}
	return (1);
}

int				parse_args(t_env *e, int ac, char **av)
{
	int show;
	int elem;

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
		if (!(check_params(e, ac, av, elem, &show)))
		{
			ft_printf("wrong champ\n");
			return (0);
		}
	}
	if (show || e->nb_champ < 2 || e->nb_champ > 4)
		ft_printf("usage:\n	%s [-dump nbr_cycles] [[-n number] champion1.cor] ...\n	You must enter between 2 and 4 champions\n	Champions must have the .cor extension\n\n", av[0]);
	return (!(show || e->nb_champ < 2 || e->nb_champ > 4));
}
