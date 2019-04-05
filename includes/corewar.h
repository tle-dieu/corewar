/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/05 13:02:20 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "op.h"
# define MAX_SIZE CHAMP_MAX_SIZE + COMMENT_LENGTH + PROG_NAME_LENGTH + 16

/* ************************************************************************** */
# include <stdio.h>
/* ************************************************************************** */

typedef struct		s_ocp
{
	int				param1;
	int				param2;
	int				param3;
}					t_ocp;

typedef struct 		s_inst
{
	int				opcode;
	int				cycle;
	struct s_inst	*next;
}					t_inst;

typedef	struct		s_proc
{
	int				id;
	int				live;
	unsigned char	registre[16];
	int				pc;
	int				carry;
	struct s_proc   *next;
}					t_proc;

typedef	struct		s_champ
{
	char			*name;
	char			*com;
	unsigned char	*content;
	int				id;
	int				size;
	t_proc			*proc;	
}					t_champ;

typedef	struct		s_env
{
	int				c;
	int				c_to_die;
	int				c_total;
	int				nb_champ;
	t_champ			*champs;
	unsigned char	*mem;
	unsigned char	*line;
	unsigned int	chmp_size;
}					t_env;

int					usage(int ac, char **av);

/*
 ** CHECK.C
 */
void				check_instructions(t_env *e);
int					check_magic_number(t_env *e);
int					check_chmp_size(t_env *e, int ret);

/*
 ** OP_CODES.C
 */
void				live(t_env *e, int *i);
void				ld(t_env *e, int *i);
void				st(t_env *e, int *i);
void				add(t_env *e, int *i);
void				sub(t_env *e, int *i);
void				and(t_env *e, int *i);
void				or(t_env *e, int *i);
void				xor(t_env *e, int *i);
void				zjmp(t_env *e, int *i);
void				ldi(t_env *e, int *i);
void				sti(t_env *e, int *i);
void				op_fork(t_env *e, int *i);
void				lld(t_env *e, int *i);
void				lldi(t_env *e, int *i);
void				lfork(t_env *e, int *i);
void				aff(t_env *e, int *i);

/*
 ** ---------DEBUG----------
 */

void				print_chmp(t_env *e, int full, int only_chmp, int cursor);
void				print_env(t_env env);

#endif
