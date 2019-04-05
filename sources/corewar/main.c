/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 13:02:17 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void place_champ(t_env *e)
{
	int part;
	int champ;
	int i;
	int j;

	champ = 0;
	part = MEM_SIZE / e->nb_champ;
	while (champ < e->nb_champ)
	{
		j = 0;
		i = champ * part;
		while (e->champs[champ].content[j])
		{
			e->mem[i] = e->champs[champ].content[j];
			++i;
			++j;
		}
		champ++;
	}
}
int					play(t_env *e)
{
	int		i;
	int		live;

	live = 0;
	i = 0;
	while (e->c < e->c_to_die)
	{
		e->c++;
		if (e->c == e->c_to_die && live)
		{
			e->c = 0;
			e->c_to_die -= CYCLE_DELTA;
		}
	}
	return (0);
}

int					main(int ac, char **av)
{
	t_env			e;
	int				i;
	int				fd;
	int				ret;
	unsigned char	mem[MEM_SIZE];
	unsigned char	line[MAX_SIZE];
	
	i = 0;
	e = (t_env){0, 1536, 0, 4, NULL, mem, line, 0};
	if (usage(ac, av))
		exit(1);
	ft_bzero(e.line, MAX_SIZE);
	ft_bzero(e.mem, MEM_SIZE);
	if (!(e.champs = (t_champ*)malloc(sizeof(t_champ) * e.nb_champ)))
		return (1);
	while (i < e.nb_champ)
	{
		e.champs[i] = (t_champ){"zork", "un commentaire", (unsigned char*)"bonjour\0", i, 7, NULL};
		i++;
	}
	fd = open(av[1], O_RDONLY);
	ret = read(fd, e.line, MAX_SIZE + 1);
	if (!(check_chmp_size(&e, ret)))
	{
		printf("Champ too big -> %d\n", e.chmp_size);
		return (0);
	}
	if (!(check_magic_number(&e)))
		printf("Wrong magic number\n");
	check_instructions(&e);
	place_champ(&e);
	play(&e);
	print_env(e);
	return (0);
}
