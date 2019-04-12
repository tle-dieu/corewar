/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/11 21:06:59 by acompagn         ###   ########.fr       */
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
# define PRINT 0
/* ************************************************************************** */

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
	int				file;
	int				id;
	int				chosen_id[2];
	unsigned int	size;
	t_proc			*proc;	
}					t_champ;

typedef	struct		s_env
{
	int				nb_live;
	int				last_live;
	int				cycle;
	int				c_to_die;
	int				c_total;
	int				nb_champ;
	t_champ			champs[4];
	unsigned char	mem[MEM_SIZE];
	unsigned char	line[MAX_SIZE];
}					t_env;

/*
 ** ARGS.C (3)
 */
int					parse_args(t_env *e, int ac, char **av);

/*
 ** CHECK.C (5)
 */
int					check_champ(t_env *e, char *arg, int i);
int					check_reg(int reg);

/*
 ** OP_UTILS.C (5)
	
 */
void				insert(t_env *e, int pc, void *ptr, int size);
int					param_sum(t_env *e, int pc, int size);
int					find_param_value(t_env *e, t_ocp check, int to_find, int *pc, t_proc *ptr);
t_ocp				check_ocp(int ocp, int on_two);

/*
 ** OP_1_5.C (5)
 */
void				live(t_env *e, int *pc, t_proc *ptr);
void				ld(t_env *e, int *pc, t_proc *ptr);
void				st(t_env *e, int *pc, t_proc *ptr);
void				add(t_env *e, int *pc, t_proc *ptr);
void				sub(t_env *e, int *pc, t_proc *ptr);

/*
 ** OP_6_10.C (5)
 */
void				and(t_env *e, int *pc, t_proc *ptr);
void				or(t_env *e, int *pc, t_proc *ptr);
void				xor(t_env *e, int *pc, t_proc *ptr);
void				zjmp(t_env *e, int *pc, t_proc *ptr);
void				ldi(t_env *e, int *pc, t_proc *ptr);

/*
 ** OP_11_15.C (5)
 */
void				sti(t_env *e, int *pc, t_proc *ptr);
void				op_fork(t_env *e, int *pc, t_proc *ptr);
void				lld(t_env *e, int *pc, t_proc *ptr);
void				lldi(t_env *e, int *pc, t_proc *ptr);
void				lfork(t_env *e, int *pc, t_proc *ptr);

/*
 ** PRINT.C (1)
 */
void				aff(t_env *e, int *pc, t_proc *ptr);

/*
 ** PLAY.C (4)
 */
int					choose_cycle(int op);
void				play(t_env *e);

/*
 ** INIT (5)
 */
void				attribute_id(t_env *e);
int					create_new_process(t_env *e, int pc, t_proc *ptr);
void				init(t_env *e);
int					init_proc(t_env *e, int j, int begin);
void				place_champ(t_env *e);


/*
 ** DEBUG (4)
 */
void				print_chmp(t_env *e, int c, unsigned int cursor);
void				print_process(t_env *e);
void				print_env(t_env env, int cursor);
void				print_split_champ(t_env *e, int i);

/*
 ** CLEAN (3)
 */
void				freedom(t_env *e);
void				destroy_process(t_env *e, int i, t_proc *to_del);
void				destroy_all(t_env *e, int i);

#endif
