/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:07:14 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/03 20:49:54 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

//option output a rajouter pour preciser fichier ou mettre

typedef struct	s_file
{
	char		*file;
	char		annot;
	char		disas;
	char		dump; //gerer hexa(-x) et binaire(-b) ?
	int			error;
	char		*msg;
}				t_file;

void			print_option(t_file *option, char *s);
#endif
