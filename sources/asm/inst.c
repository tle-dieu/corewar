/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:12:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/24 10:42:04 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "asm.h"
#include "op.h"

int     is_label(char *s)
{
	int		i;
	int		len;

	len = sizeof(SPACES) - 1;
	while (*s != LABEL_CHAR)
	{
		i = len;
		if (!*s)
			return (0);
		while (i--)
			if (SPACES[i] == *s)
				return (0);
		s++;
	}
	return (1);
}

int		get_label(t_env *e, char **s, t_label **label, int i)
{
	t_label	*new;
	int		len;

	if (*((*s) + (len = ft_strspn(*s, LABEL_CHARS))) != LABEL_CHAR)
		ft_printf("{#ff3333}label char interdit: {R}%c\n", *((*s) + len));
	new = NULL;
	if (!(new = (t_label *)malloc(sizeof(t_label))))
		alloc_error(e);
	if (!(new->name = ft_strndup(*s, len)))
	{
		free(new);
		alloc_error(e);
	}
	ft_printf("new label: %s\n", new->name);
	new->index = i;
	new->call = NULL;
	new->next = *label;
	*label = new;
	return (1);
}

int		get_champ(t_env *e, char *s, unsigned char *cp)
{
	t_label *label;
	int		i;

	i = 0;
	if (is_label(s))
	{
		ft_printf(NAME_C"it's a label{R}\n");
		if (!get_label(e, &s, &label, i))
			return (0);
		s = ft_strchr(s, LABEL_CHAR);
	}
	else
	{
		ft_printf(NAME_C"it's an instruction{R}\n");
		parse_inst(s);
	}
	(void)e;
	(void)cp;
	return (1);
}
