/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/10 19:38:01 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "libft.h"
# define RED_ERR "{bold}{rgb(255,109,103)}"
# define GREEN_CURS "{bold}{rgb(95,250,104)}"
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
void				print_option(t_file *option, char *s);
void				print_files(t_file *file);
void				print_bin(unsigned char *buff, int size);

# define FT_C "{#2ecc71}"

/*
** ---------- FREE -----------
*/

void				free_lst_file(t_file *file);

/*
** --------- OPTION ----------
*/

t_file				*parse_command_line(int ac, char **av);

/*
** ---------- ERROR ----------
*/

int					usage(char *ex_name, int help);
void				*alloc_error(char *ex_name);
int					error_file(t_file *option, char *ex_name,
					char *file, t_file *lst);
#endif
