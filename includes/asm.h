/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/08 18:07:52 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "libft.h"
# define RED_ERR "{bold}{#ff6b6b}"
# define BS_ASM 1000
//option output a rajouter pour preciser fichier ou mettre

typedef struct		s_file
{
	char			*name;
	int				fd;
	char			annot;
	char			disas;
	char			dump; //gerer hexa(-x) et binaire(-b) ?
	int				error;
	char			*msg;
	struct s_file	*next;
}					t_file;

/*
** ---------- DEBUG ----------
*/
void				print_option(t_file *option, char *s);
void				print_files(t_file *file);
void				print_bin(unsigned char *buff, int size);

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
