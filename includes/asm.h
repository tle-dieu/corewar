/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/18 00:20:22 by tle-dieu         ###   ########.fr       */
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

# define COLOR_LINE(x) (x ? ERR_LINE_C : ERR_LINE_NC)
# define COLOR_FILE(x) (x ? ERR_FILE_C : ERR_FILE_NC)

# define TAB_SIZE 8
//option output a rajouter pour preciser fichier ou mettre

# define O_ANNOT 1
# define O_DUMP 2 //gerer hexa(-x) et binaire(-b) ?
# define O_DISAS 4
# define O_SHORT_ERR 128
# define O_LONG_ERR 256

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
	int				fatal_error;
	int				error;
	t_line			*begin;
	t_line			*last;
	struct s_file	*next;
}					t_file;


typedef struct		s_env
{
	int				tty;
	int				line_nb;
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


//----------------------------


int					usage(t_env *e, int err);
void				free_lst_file(t_file *file);
int					parse_command_line(t_env *e, int ac, char **av);
void				err_pointer(int tty, char *s, char *end);
void				err_wave(int tty, char *s);
char				*check_end_str(char **end);
int					error_file(t_env *e, char *error, char *file, unsigned opt);
void				put_strtab(char *s, char replace, int n);
int					error_header(t_env *e, int error, char *extra, int cmd);
void				free_line(t_file *file);
int					add_line(t_env *e, char **line);
int					get_header(t_env *e, unsigned char *cp);
int					alloc_error(t_env *e);

//verifier include dans .c et verifier proto fonctions

#endif
