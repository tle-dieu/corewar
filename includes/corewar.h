/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/09 18:01:44 by acompagn         ###   ########.fr       */
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

typedef struct		s_ocp
{
	int				p1;
	int				s1;
	int				p2;
	int				s2;
	int				p3;
	int				s3;
}					t_ocp;

typedef	struct		s_proc
{
	int				owner;
	int				id;
	int				live;
	int				r[17];
	int				pc;
	int				carry;
	int				op;
	int				cycle;
	struct s_proc   *next;
}					t_proc;

typedef	struct		s_champ
{
	char			name[PROG_NAME_LENGTH];
	char			comment[COMMENT_LENGTH];
	unsigned char	content[CHAMP_MAX_SIZE];
	int				alive;
	int				file;
	int				id;
	int				chosen_id[2];
	unsigned int	size;
	t_proc			*proc;	
}					t_champ;

typedef	struct		s_env
{
	int				nb_live;
	int				cycle;
	int				living;
	int				c_to_die;
	int				c_total;
	int				nb_champ;
	t_champ			champs[4];
	unsigned char	mem[MEM_SIZE];
	unsigned char	line[MAX_SIZE];
}					t_env;

int					parse_args(t_env *e, int ac, char **av);

/*
 ** CHECK.C
 */
int					check_champ(t_env *e, char *arg, int i);

/*
 ** OP_CODES.C
	
 */
t_ocp				check_ocp(int ocp, int on_two);
void				move_pc(t_ocp check, int *pc, int on_two);
int					param_sum(t_env *e, int pc, int size);
void				live(t_env *e, int *pc, t_proc *ptr);
void				ld(t_env *e, int *pc, t_proc *ptr);
void				st(t_env *e, int *pc, t_proc *ptr);
void				add(t_env *e, int *pc, t_proc *ptr);
void				sub(t_env *e, int *pc, t_proc *ptr);
void				and(t_env *e, int *pc, t_proc *ptr);
void				or(t_env *e, int *pc, t_proc *ptr);
void				xor(t_env *e, int *pc, t_proc *ptr);
void				zjmp(t_env *e, int *pc, t_proc *ptr);
void				ldi(t_env *e, int *pc, t_proc *ptr);
void				sti(t_env *e, int *pc, t_proc *ptr);
void				op_fork(t_env *e, int *pc, t_proc *ptr);
void				lld(t_env *e, int *pc, t_proc *ptr);
void				lldi(t_env *e, int *pc, t_proc *ptr);
void				lfork(t_env *e, int *pc, t_proc *ptr);
void				aff(t_env *e, int *pc, t_proc *ptr);

/*
 ** ---------PLAY----------
 */

void				play(t_env *e);

/*
 *
 ** ---------DEBUG----------
 */

void				print_chmp(t_env *e, int c, unsigned int cursor);
void				print_env(t_env env);
void				print_split_champ(t_env *e, int i);

#endif
