/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/21 04:27:28 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "libft.h"
# define GREEN_CURS "{bold}{green}"

# define ERR_LINE_C ("{bold}%s:%d:%d: {bold}{red}error: {R}{bold}")
# define ERR_LINE_NC ("%s:%d:%d: error: ")

# define ERR_FILE_C ("%s: {bold}{red}error: {R}")
# define ERR_FILE_NC ("%s: error: ")

# define ERR_FATAL_C ("{bold}{red}fatal error: {R}")
# define ERR_FATAL_NC ("fatal error: ")

# define COLOR_LINE(x) (x ? ERR_LINE_C : ERR_LINE_NC) //remplacer ca par une fonction degeulasse
# define COLOR_FILE(x) (x ? ERR_FILE_C : ERR_FILE_NC)
# define COLOR_FATAL(x) (x ? ERR_FATAL_C : ERR_FATAL_NC)

# define SPACES "\t \v\f\r"
# define TAB_SIZE 4
//option output a rajouter pour preciser fichier ou mettre

# define O_ANNOT 1
# define O_DUMP 2 //gerer hexa(-x) et binaire(-b) ?
# define O_DISAS 4
# define O_SHORT_ERR 128
# define O_LONG_ERR 256
# define O_COL_AMBIGUOUS_ERR 512
# define O_COL_INVALID_ERR 1024

typedef struct		s_line
{
	char			*s;
	int				y;
	struct s_line	*next;
}					t_line;

typedef struct		s_file
{
	char			complete;
	char			*name;
	int				fd;
	unsigned		options;
	int				error;
	int				line_nb;
	int				nb_inst;
	t_line			*begin;
	t_line			*last;
	struct s_file	*next;
}					t_file;


typedef struct		s_env
{
	int				tty;
	t_file			*actual;
	t_file			*file;
	char			*exname;
}					t_env;

typedef struct		s_inst
{
	int				op;
	int				ocp;
	int				nb_p;
	int				p[3];
	int				s[3];
	int				t[3];
	struct s_inst 	*next;
}					t_inst;

/*
 ** ---------- DEBUG ----------
 */

# define FT_C "{#2ecc71}"
# define STR_C "{#0bd195}"
# define NAME_C "{#f1c40f}"
# define COMMENT_C "{#3498f0}"
# define CHAMP_C "{#e74c3c}"
# define SIZE_C "{#2ecc71}"
# define MAGIC_C "{#9b59b6}"

void				print_files(t_file *file);
void				print_bin(unsigned char *buff, int size);
void	            print_option(unsigned options, char *s);


//----------------------------


int					usage(t_env *e, int err);
int					get_inst(t_env *e, char *s, unsigned char *cp);
void				free_lst_file(t_file *file);
int					parse_command_line(t_env *e, int ac, char **av);
void				err_pointer(int tty, char *s, char *end, int sp);
void				err_wave(int tty, char *s, int n);
char				*check_end_str(char **end);
int					error_file(t_env *e, char *error, char *file, unsigned opt);
int					error_header(t_env *e, int error, char *extra, int cmd);
void				free_line(t_file *file);
int					add_line(t_env *e, char **line);
void				get_header(t_env *e, unsigned char *cp);
int					alloc_error(t_env *e);
t_inst				*parse_inst(char *str);

//verifier include dans .c et verifier proto fonctions

#endif
