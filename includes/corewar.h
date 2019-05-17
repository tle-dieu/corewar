/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:32:19 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/17 15:07:47 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H
# include "libft.h"
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
# define PROGRESS_BAR_SIZE 191

typedef struct		s_ocp
{
	int				error;
	int				shift;
	int				v[3];
	int				p[3];
	int				s[3];
}					t_ocp;

typedef	struct		s_proc
{
	int				owner;
	int				dead;
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
	ssize_t			size;
}					t_champ;

typedef struct		s_visu
{
	int				posess[4];
	int				map[MEM_SIZE];
	int				color;
	int				live_color;
	unsigned int	sleep_value;

}					t_visu;

typedef	struct		s_env
{
	int				dump;
	int				verbose;
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
	t_champ			champs[4];
	t_proc			*proc;
	t_proc			*new_proc;
	t_proc			*proc_to_load;
	int				nb_proc;
	unsigned char	mem_cpy[MEM_SIZE];
	unsigned char	mem[MEM_SIZE];
	unsigned char	line[MAX_SIZE];
}					t_env;

/*
** VISU.C (3)
*/
void				get_keys(t_env *e);
void				visu(t_env *e);

/*
** VISU_PRINT.C (4)
*/
void				print_current_map(t_env *e);

/*
** OP.C ()
*/

/*
** ARGS.C (4)
*/
int					parse_args(t_env *e, int ac, char **av);

/*
** CHECK.C (5)
*/
int					check_champ(t_env *e, char *arg, int i);

/*
** GAME_UTILS.C (3)
*/
void				print_verbosity(t_env *e, t_proc *ptr);
int					create_new_process(t_env *e, int pc, t_proc *ptr);
int					check_reg(int reg);

/*
** OCP_UTILS.C (3)
*/
t_ocp				check_ocp(int ocp, int on_two, int inst);

/*
** OP_UTILS.C (3)
*/
void				insert(t_env *e, int pc, void *ptr, int size);
int					param_sum(t_env *e, int pc, int size);
void				param_value(t_env *e, t_ocp *ch, t_proc *ptr, int mod);

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
** PLAY.C (5)
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
** CLEAN.C (2)
*/
void				freedom(t_env *e, int to_exit);

extern t_op			g_op_tab[17];

#endif
