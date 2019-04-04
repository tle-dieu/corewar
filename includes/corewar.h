/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/04 16:42:29 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H

typedef struct s_inst
{
	int				opcode;
	int				cycle;
	struct s_inst	*next;
}				t_inst;

typedef	struct s_proc
{
	int				id;
	int				live;
	unsigned char	registre[16];
	int				pc;
	int				carry;
	struct s_proc   *next;
}				t_proc;

typedef	struct	s_champ
{
	char			*name;
	char			*com;
	unsigned char	*content;
	int				id;
	int				size;
	t_proc			*proc;	
}				t_champ;

typedef	struct	s_env
{
	int				c;
	int				c_to_die;
	int				c_total;
	int				nb_champ;
	t_champ			*champs;
	unsigned char	*mem;
}				t_env;

int	usage(int ac, char **av);

/*
** ---------DEBUG----------
*/

void    print_env(t_env env);
#endif
