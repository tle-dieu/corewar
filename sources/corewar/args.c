/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:04 by acompagn          #+#    #+#             */
/*   Updated: 2019/06/21 12:23:49 by acompagn         ###   ########.fr       */
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

static void		save_dump(t_env *e, char *av, int *show, int *i)
{
	e->dump = ft_atoi(av);
	*show = (e->dump < 0) ? 1 : *show;
	*i += 2;
}

static void		verbosity_on(t_env *e, char *av, int *show, int *i)
{
	e->verbose = ft_atoi(av);
	*show = (e->verbose < 0 || e->verbose > 4) ? 1 : *show;
	*i += 2;
}

static void		check_params(t_env *e, int ac, char **av, int *show)
{
	char	*tmp;
	int		i;

	i = 1;
	while (i != ac && !*show)
	{
		if (!ft_strcmp(av[i], "-n") && ac > i + 2 && digit(av[i + 1])
			&& e->nb_champ < 4 && (i += 2))
		{
			e->champs[e->nb_champ].chosen_id[0] = 1;
			e->champs[e->nb_champ].chosen_id[1] = ft_atoi(av[i - 1]);
		}
		else if (!ft_strcmp(av[i], "-v") && (e->visu = 1))
			i++;
		else if (ac > i + 1 && !ft_strcmp(av[i], "-verbose")
			&& digit(av[i + 1]))
			verbosity_on(e, av[i + 1], show, &i);
		else if (ac > i + 1 && !ft_strcmp(av[i], "-dump") && digit(av[i + 1]))
			save_dump(e, av[i + 1], show, &i);
		else if ((tmp = ft_strrchr(av[i], '.')) && !ft_strcmp(tmp, ".cor")
			&& e->nb_champ < 4)
			e->champs[e->nb_champ++].file = i++;
		else
			*show = 1;
	}
}

int				parse_args(t_env *e, int ac, char **av)
{
	int		show;

	show = 1;
	e->verbose = 0;
	if (ac > 2)
	{
		show = 0;
		check_params(e, ac, av, &show);
	}
	if (show || e->nb_champ < 2 || e->nb_champ > 4)
		usage(av[0]);
	return (!(show || e->nb_champ < 2 || e->nb_champ > 4));
}
