/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/04 13:25:26 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H

int	usage(int ac, char **av);

typedef	struct	s_champ
{
	char			*name;
	char			*desc;
	int				id;
	int				size;
}				t_champ;

typedef	struct	s_en
{
	int				c;
	int				c_to_die;
	int				c_total;
	t_champ			*champs;
	unsigned char **map;
}				t_env;

typedef struct s_inst
{
	int				opcode;
	int				cycle;
	struct s_inst	next;
}				t_inst
#endif
