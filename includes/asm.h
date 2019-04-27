/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/27 14:07:55 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

// refaire usage et --help

# include "libft.h"
# define GREEN_CURS "{bold}{green}"

# define ERR_LINE_C ("{bold}%s:%d:%d: {bold}{red}error: {R}{bold}")
# define ERR_LINE_NC ("%s:%d:%d: error: ")

# define WARNING_LINE_C ("{bold}%s:%d:%d: {bold}{purple}warning: {R}{bold}")
# define WARNING_LINE_NC ("%s:%d:%d: warning: ")

# define WARNING_FILE_C ("{bold}%s: {bold}{purple}warning: {R}")
# define WARNING_FILE_NC ("%s: warning: ")

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
//supprimer options inutiles

# define O_ANNOT 1
# define O_DUMP 2 //gerer hexa(-x) et binaire(-b) ?
# define O_DISAS 4
# define O_SHORT_ERR 128
# define O_LONG_ERR 256
# define O_COL_AMBIGUOUS_ERR 512
# define O_COL_INVALID_ERR 1024
# define PRINT 1
typedef struct		s_line
{
	char			*s;
	int				y;
	int				free;
	struct s_line	*next;
}					t_line;

typedef struct		s_call
{
	t_line			*line;
	char			*s;
	int				size;
	int				index_call;
	int				index_inst;
	struct s_call	*next;
}					t_call;

typedef struct		s_label
{
	char			*name;
	int				index;
	t_call			*call;
	struct s_label	*next;
}					t_label;

typedef struct		s_inst
{
	int				index;
	int				op;
	int				ocp;
	int				nb_p;
	int				p[3];
	int				s[3];
	int				t[3];
}					t_inst;

typedef struct		s_file
{
	char			complete;
	char			*name;
	int				fd;
	int				i;
	unsigned		options;
	int				error;
	int				line_nb;
	int				nb_inst;
	t_line			*begin;
	t_line			*last;
	t_label			*label;
	struct s_file	*next;
}					t_file;

typedef struct		s_env
{
	int				tty;
	t_file			*actual;
	t_file			*file;
	char			*exname;
}					t_env;

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
void				print_label(t_env *e);
void				print_call_error(t_env *e);


//----------------------------


int					usage(t_env *e, int err);
int					get_champ(t_env *e, char *s, unsigned char *cp);
void				free_lst_file(t_env *e);
int					parse_command_line(t_env *e, int ac, char **av);
void				err_pointer(int tty, char *s, char *end, int sp);
void				err_wave(int tty, char *s, int n);
int					check_end_str(t_env *e, char *s, int cmd, char c);
int					error_file(t_env *e, char *error, char *file, unsigned opt);
int					error_header(t_env *e, int error, char *extra, int cmd);
void				free_line(t_file *file);
int					add_line(t_env *e, char **line);
void				get_header(t_env *e, unsigned char *cp);
int					alloc_error(t_env *e);
t_inst				*parse_inst(t_env *e, char *str, unsigned char *cp);
//verifier include dans .c et verifier proto fonctions

#endif
