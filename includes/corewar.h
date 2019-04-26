/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/26 11:50:46 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <ncurses.h>
# include "op.h"
# define NAME_COMM_SIZE PROG_NAME_LENGTH + COMMENT_LENGTH
# define MAX_SIZE CHAMP_MAX_SIZE + NAME_COMM_SIZE + 16
# define PLAYER_1 2
# define PLAYER_2 3
# define PLAYER_3 4
# define PLAYER_4 5
# define NO_ONE 6
# define WRITING 7
# define PROGRESS_1 8
# define PROGRESS_2 9
# define PROGRESS_3 10
# define PROGRESS_4 11
# define PROGRESS_NO 12
# define PROGRESS_BAR_SIZE 192

typedef struct		s_ocp
{
	int				error;
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
	int				color;
	int				id;
	int				live;
	int				r[17];
	int				pc;
	int				carry;
	int				op;
	int				cycle;
	struct s_proc	*next;
}					t_proc;

typedef	struct		s_champ
{
	char			name[PROG_NAME_LENGTH];
	char			comment[COMMENT_LENGTH];
	unsigned char	content[CHAMP_MAX_SIZE];
	int				nb_live;
	int				file;
	int				color;
	int				id;
	int				chosen_id[2];
	int				size;
}					t_champ;

typedef struct		s_visu
{
	int				posess[4];
	int				color_map[MEM_SIZE];
	int				color;
	int				live_color;
	unsigned int	sleep_value;

}					t_visu;

typedef struct		s_decomp
{
	char			tab[300][COMMENT_LENGTH];
	int				y;
	int				x;
	int				i;
	int				op;
	int				champ;
}					t_decomp;

typedef	struct		s_env
{
	int				dump;
	int				visu;
	int				total_live;
	int				total_proc;
	int				nb_live;
	int				nb_check;
	int				last_live;
	int				cycle;
	int				c_to_die;
	int				c_total;
	int				nb_champ;
	t_visu			v;
	t_decomp		d;
	t_champ			champs[4];
	t_proc			*proc;
	t_proc			*new_proc;
	int				nb_proc;
	unsigned char	mem_cpy[MEM_SIZE];
	unsigned char	mem[MEM_SIZE];
	unsigned char	line[MAX_SIZE];
}					t_env;

/*
** VISU.C (3)
*/
void				get_keys(t_env *e);
void				init_color_map(t_env *e);
void				visu(t_env *e);

/*
** VISU_PRINT.C (4)
*/
void				print_current_map(t_env *e);

/*
** OP.C ()
*/

/*
** ARGS.C (3)
*/
int					parse_args(t_env *e, int ac, char **av);

/*
** CHECK.C (4)
*/
int					check_champ(t_env *e, char *arg, int i);

/*
** GAME_UTILS.C (2)
*/
int					create_new_process(t_env *e, int pc, t_proc *ptr);
int					check_reg(int reg);

/*
** OP_UTILS.C (5)
*/
void				insert(t_env *e, int pc, void *ptr, int size);
int					param_sum(t_env *e, int pc, int size);
int					param_value(t_env *e, t_ocp check, int to_find,
		t_proc *ptr);
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
** PRINT.C (5)
*/
void				usage(char *path);
void				print_memory(t_env *e, int cursor);
void				print_subject_winner(t_env *e);
void				print_winner(t_env *e);
void				aff(t_env *e, int *pc, t_proc *ptr);

/*
** PLAY.C (4)
*/
int					choose_cycle(int op);
void				play(t_env *e);

/*
** INIT.C (5)
*/
void				check_taken_id(t_env *e);
void				attribute_id(t_env *e);
void				init(t_env *e);
int					init_proc(t_env *e, int j, int begin);
void				place_champ(t_env *e);

/*
** DEBUG.C (3)
*/
void				print_game(t_env *e, t_proc *ptr);
void				print_chmp(t_env *e, int c, int cursor);
void				print_split_champ(t_env *e, int i);

/*
** CLEAN.C (2)
*/
void				freedom(t_env *e);
void				destroy_process(t_env *e, t_proc *to_del);

/*
** DECOMP.C (5)
*/
int					decompile_champ(t_env *e, int champ);

/*
** DECOMP_UTILS.C (6)
*/
void				init_decomp(t_decomp *d, int champ);
void				print_tab(t_decomp *d);
void				init_line(t_env *e);
void				nb_in_buff(t_decomp *d, int nb, int padding);
void				str_in_buff(t_decomp *d, char *s);
int					compute_param(t_env *e, int champ, int i, int size);

extern t_op			g_op_tab[17];

#endif
