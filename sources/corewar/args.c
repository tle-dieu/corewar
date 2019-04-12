/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:04 by acompagn          #+#    #+#             */
/*   Updated: 2019/04/12 11:17:59 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static int		digit(char *str)
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

static void		check_params(t_env *e, int ac, char **av, int *show)
{
	char	*tmp;
	int		i;

	i = 1;
	*show = 0;
	while (i != ac && !*show)
	{
		if (!ft_strcmp(av[i], "-n") && ac > i + 2 && digit(av[i + 1]))
		{
			e->champs[e->nb_champ].chosen_id[0] = 1;
			e->champs[e->nb_champ].chosen_id[1] = ft_atoi(av[i + 1]);
			i += 2;
		}
		else if (ac > i + 1 && !ft_strcmp(av[i], "-dump") && digit(av[i + 1]))
		{
			e->dump = ft_atoi(av[i + 1]);
			*show = (e->dump < 0) ? 1 : *show;
			i += 2;
		}
		else if ((tmp = ft_strrchr(av[i], '.')) && !ft_strcmp(tmp, ".cor"))
			e->champs[e->nb_champ++].file = i++;
		else
			*show = 1;
	}
}

int				parse_args(t_env *e, int ac, char **av)
{
	int		show;

	show = 1;
	if (ac > 2)
		check_params(e, ac, av, &show);
	if (show || e->nb_champ < 2 || e->nb_champ > 4)
		ft_printf("usage:\n	%s [-dump nbr_cycles] [[-n number] champion1.cor] ...\n	You must enter between 2 and 4 champions\n	Champions must have the .cor extension\n\n", av[0]);
	return (!(show || e->nb_champ < 2 || e->nb_champ > 4));
}
