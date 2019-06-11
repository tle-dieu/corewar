/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 12:41:36 by matleroy          #+#    #+#             */
/*   Updated: 2019/06/11 02:14:00 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void			init_inst(t_inst *inst)
{
	inst->index = 0;
	inst->i = 0;
	inst->error = 0;
	inst->op = 0;
	inst->ocp = 0;
	inst->nb_p = 0;
	ft_bzero(inst->p, 3);
	ft_bzero(inst->s, 3);
	ft_bzero(inst->t, 3);
}

size_t			param_strrspn(const char *s, const char *accept, char stop)
{
	const char	*tmp;
	size_t		i;

	i = ft_strclen(s, stop) - 1;
	ft_printf("%p %p\n", s, accept); // a retirer
	//sur le fichier ./resources/corewar_42 variable s overflow: (addr de s = 0x602000094bd3 dans l'exemple)
	//    ^0x602000094bd3 0x585720
	//sources/asm/utils.c:44:18: runtime error: addition of unsigned offset to 0x602000094bd3 overflowed to 0x602000094bd2
	//SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior sources/asm/utils.c:44:18 in 
	//depend de l'addr choisie par donc doit recompiler si pas d'erreur
	while (i > 0)
	{
		tmp = accept;
		while (*tmp != s[i])
			if (!*tmp++)
				return (i);
		--i;
	}
	return (i);
}

unsigned char	*lst_to_char(t_env *e, unsigned char *header, int *size)
{
	unsigned char	*str;
	t_buff			*buff;
	int				i;
	int				j;

	buff = e->file->begin_buff;
	*size = HEADER_SIZE;
	while (buff)
	{
		*size += buff->len;
		buff = buff->next;
	}
	if (!(str = (unsigned char *)malloc(sizeof(unsigned char) * (*size))))
		alloc_error(e);
	i = 0;
	while (i < HEADER_SIZE)
		str[i++] = *header++;
	buff = e->file->begin_buff;
	while (buff && !(j = 0))
	{
		while (j < buff->len)
			str[i++] = buff->s[j++];
		buff = buff->next;
	}
	return (str);
}
