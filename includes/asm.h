/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/03 14:22:37 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

// refaire usage et --help

# include "libft.h"

# define DEFAULT_NAME "no name"
# define DEFAULT_COMMENT "description"

/* # define BS_ASM		4096 */
# define BS_ASM		64 // remettre a 4096 mais mieux pour tester bug et segfaults
# define GREEN_CURS "{bold}{green}"

# define ERR_LINE 1
# define ERR_LINE_C ("{bold}%s:%d:%d: {bold}{red}error: {R}{bold}")
# define ERR_LINE_NC ("%s:%d:%d: error: ")

# define ERR_FILE 2
# define ERR_FILE_C ("%s: {bold}{red}error: {R}")
# define ERR_FILE_NC ("%s: error: ")

# define ERR_FATAL 3
# define ERR_FATAL_C ("{bold}{red}fatal error: {R}")
# define ERR_FATAL_NC ("fatal error: ")

# define WARNING_LINE 4
# define WARNING_LINE_C ("{bold}%s:%d:%d: {bold}{purple}warning: {R}{bold}")
# define WARNING_LINE_NC ("%s:%d:%d: warning: ")

# define WARNING_FILE 5
# define WARNING_FILE_C ("{bold}%s: {bold}{purple}warning: {R}")
# define WARNING_FILE_NC ("%s: warning: ")

# define SPACES " \t\r\v\f"
# define TAB_SIZE 4
# define MAX_ERROR 20
# define MAX_FILE_SIZE 100000000

# define PRINT 0 // a enlever

# define NAME_CMD 1
# define COMMENT_CMD 2
# define ALREADY_DEFINE 128

# define O_HEXA 1
# define O_BIN 2 
# define O_DISAS 4
# define O_OUTPUT 8
# define O_LONG 16
# define O_SHORT_ERR 256
# define O_LONG_ERR 512
# define O_COL_AMBIGUOUS_ERR 1024
# define O_COL_INVALID_ERR 2048
# define O_OUTPUT_ERR 4096
# define O_INVALID_FILE_ERR 8192

typedef	struct		s_buff
{
	unsigned char	s[BS_ASM];	
	long			len;
	struct s_buff	*next;
}					t_buff;

typedef struct		s_line
{
	char			*s;
	int				y;
	int				free;
}					t_line;

typedef struct		s_call
{
	t_line			*line;
	char			*s;
	int				size;
	long			index_call;
	long			index_inst;
	struct s_call	*next;
}					t_call;

typedef struct		s_label
{
	char			*name;
	long			index;
	t_call			*call;
	struct s_label	*next;
}					t_label;

typedef struct		s_inst
{
	long			index;
	long			i;
	int				error;
	int				op;
	int				ocp;
	int				nb_p;
	int				p[3];
	int				s[3];
	int				t[3];
}					t_inst;

typedef struct		s_file
{
	t_buff			*begin_buff;
	t_buff			*buff;
	char			complete;
	char			*output;
	char			*name;
	int				fd;
	long			i;
	unsigned		options;
	int				error;
	int				warning;
	int				line_nb;
	int				too_long;
	t_line			*begin;
	t_line			*last;
	t_label			*label;
	struct s_file	*next;
}					t_file;

typedef struct		s_env
{
	int				tty1;
	int				tty2;
	int				i;
	t_file			*actual;
	t_file			*file;
	char			*exname;
	char			*output;
}					t_env;

/*
** ---------- DEBUG ----------
*/

# define FT_C "{#2ecc71}" // a suppr
# define STR_C "{#0bd195}" // a suppr
# define NAME_C "{#f1c40f}"
# define COMMENT_C "{#3498f0}"
# define CHAMP_C "{#e74c3c}"
# define SIZE_C "{#2ecc71}"
# define MAGIC_C "{#9b59b6}"

void				print_files(t_file *file);
void				print_bin(t_env *e, unsigned char *header);
void				print_label(t_env *e);
void				print_call_error(t_env *e);

//----------------------------

int					cmd_too_long(t_env *e, char *error, int cmd);
int					cmd_multiple_define(t_env *e, int cmd);
int					invalid_cmd(t_env *e, char *error, int cmd);
int					expect_str(t_env *e, char *error, int cmd);
int					unexpected_expression(t_env *e, char *error, int cmd);
int					missing_quote(t_env *e, char *error);
void				free_lst_file(t_env *e);
void				free_file(t_file **file);
char				*line_error(int line, int tty);
int					usage(t_env *e, int err);
int					only_label(t_env *e, char **line);
int					parse_command_line(t_env *e, int ac, char **av);
void				err_pointer(int tty, char *s, char *end, int sp);
void				err_wave(int tty, char *s, int n);
int					check_end_str(t_env *e, char *s, int cmd, char c);
int					error_file(t_env *e, char *error, char *file, unsigned opt);
int					add_line(t_env *e, char **line);
void				free_line(t_line **line);
void				get_bytecode(t_env *e, unsigned char *header);
int					alloc_error(t_env *e);
t_inst				*parse_inst(t_env *e, char *str);
void				basic_error(t_env *e, char *str, char *err_string, int wave);
void				print_buff(t_buff *buff);
//verifier include dans .c et verifier proto fonctions

/*
** ---------- ERROR_INST ----------
*/

void				error_nb_param(t_env *e, char *str, int have, int should_have);
void				error_param_type(t_env *e, t_inst *inst, char *str);
void				error_register_nb(t_env *e, char *str, int nb);
void				error_unknow_inst(t_env *e, char *str);

/*
** ---------- UTILS ----------
*/

int					inst_atoi(char *str);
int					is_a_number(t_env *e, char *str);
size_t				param_strrspn(const char *s, const char *accept, char stop);

/*
**	---------- LABEL ----------
*/

int					create_call(t_env *e, t_inst *inst, char *s, t_label *label, int i);
void				get_label_call(t_env *e, t_inst *inst, char *s, int i);
int					label_is_good(t_env *e, char *str);

/*
** ---------- CHECK_INST ----------
*/

int					is_reg(t_env *e, char *str, t_inst *inst);
int					is_valid_register(t_env *e, char *str);
int					is_indirect(t_env *e, char *str, t_inst *inst);
int					is_direct(t_env *e, char *str, t_inst *inst);
int					check_params(t_env *e, char *str, t_inst *inst);

#endif
