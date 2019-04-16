/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/16 23:31:04 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "libft.h"
# define RED_ERR "{bold}{red}"
# define GREEN_CURS "{bold}{green}"
# define ERR_LINE_C "{bold}%s:%d:%d: " RED_ERR "error: {R}{bold}"
# define ERR_LINE_NC "%s:%d:%d: error: "
# define TAB_SIZE 8
//option output a rajouter pour preciser fichier ou mettre

typedef struct		s_line
{
	char			*s;
	int				y;
	struct s_line	*next;
}					t_line;

typedef struct		s_file
{
	char			*name;
	int				fd;
	char			annot;
	char			disas;
	char			dump; //gerer hexa(-x) et binaire(-b) ?
	int				fatal_error;
	int				error;
	t_line			*begin;
	t_line			*last;
	struct s_file	*next;
}					t_file;


typedef struct		s_env
{
	int				tty;
	int				color;
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

void				print_option(t_file *option, char *s);
void				print_files(t_file *file);
void				print_bin(unsigned char *buff, int size);

//----------------------------


void				free_lst_file(t_file *file);
int					parse_command_line(t_env *e, int ac, char **av);
int					usage(char *ex_name, int help);
int					error_file(t_file *option, char *ex_name, char *file, t_file *lst);
void				err_pointer(char *s, char *end);
void				err_wave(char *s);
char				*check_end_str(char **end);
void				put_strtab(char *s, char replace);
int					error_header(t_file *file, int error, char *extra, int cmd);
void				free_line(t_file *file);
int					add_line(t_env *e, char **line);
int					get_header(t_env *e, unsigned char *cp);
int					alloc_error(t_env *e);

//verifier include dans .c et verifier proto fonctions

#endif
