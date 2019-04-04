/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 13:55:00 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/04 20:05:10 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "corewar.h"
#include "op.h"
#include "libft.h"

void place_champ(t_env *env)
{
	int part;
	int champ;
	int i;
	int j;

	champ = 0;
	part = MEM_SIZE / env->nb_champ;
	while (champ < env->nb_champ)
	{
		j = 0;
		i = champ * part;
		while (env->champs[champ].content[j])
		{
			env->mem[i] = env->champs[champ].content[j];
			++i;
			++j;
		}
		champ++;
	}
}
int play(t_env *env)
{
	int i;
	int live;

	live = 0;
	i = 0;
	while (env->c < env->c_to_die)
	{
		env->c++;
		if (env->c == env->c_to_die && live)
		{
			env->c = 0;
			env->c_to_die -= CYCLE_DELTA;
		}
	}
	return (0);
}

int main(int ac, char **av)
{
	t_env env;
	unsigned char mem[MEM_SIZE];
	int i;

	i = 0;
	env = (t_env){0, 1536, 0, 4, NULL, mem};
	if (usage(ac, av))
		exit(1);
	ft_bzero(env.mem, MEM_SIZE);
	if (!(env.champs = (t_champ*)malloc(sizeof(t_champ) * env.nb_champ)))
		return (1);
	while (i < env.nb_champ)
	{
		env.champs[i] = (t_champ){"zork", "un commentaire", (unsigned char*)"bonjour\0", i, 7, NULL};
		i++;
	}
	place_champ(&env);
	play(&env);
	print_env(env);
	return (0);
}
