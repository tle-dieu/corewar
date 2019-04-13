/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/13 15:30:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "libft.h"
# define RED_ERR "{bold}{red}"
# define GREEN_CURS "{bold}{green}"
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
	int				error;
	t_line			*begin;
	t_line			*last;
	struct s_file	*next;
}					t_file;

/*
** ---------- DEBUG ----------
*/

# define FT_C "{#2ecc71}"

void				print_option(t_file *option, char *s);
void				print_files(t_file *file);
void				print_bin(unsigned char *buff, int size);

//----------------------------


void				free_lst_file(t_file *file);
t_file				*parse_command_line(int ac, char **av);
int					usage(char *ex_name, int help);
void				*alloc_error(char *ex_name);
int					error_file(t_file *option, char *ex_name, char *file, t_file *lst);
int					get_header(t_file *file, unsigned char *cp);
void				err_pointer(char *s, char *end);
void				err_wave(char *s);
char				*check_end_str(char **end);
void				put_strtab(char *s, char replace);
int					error_header(t_file *file, int error, char *extra, int cmd);
int					add_line(char **line, t_file *file);

//verifier include dans .c et verifier proto fonctions

#endif
