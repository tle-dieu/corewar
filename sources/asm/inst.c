/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/20 16:19:22 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"

int		is_label(char *s)
{
	char	*end;
	int		word;

	word = 0;
	if (!(end = ft_strchr(s, LABEL_CHAR)))
		return (0);
	while (s != end && *s)
	{
		if (*s != ' ' && *s != '\t')
		{
			word++;
			while ((s + 1) != end && *(s + 1) != ' ' && *(s + 1) != '\t')
				s++;
		}
		s++;
	}
	return (word == 1);
}

int		get_inst(t_env *e, char *s, unsigned char *cp)
{
	if (is_label(s))
		ft_printf(NAME_C"it's a label\n");
	else
		ft_printf(NAME_C"it's an instruction\n");
	(void)e;
	(void)cp;
	return (1);
}
