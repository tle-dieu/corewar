/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_label.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 00:13:44 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/09 02:50:19 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"

void	redefine_label(t_env *e, char *error, int y)
{
	t_line *line;

	line = e->file->last;
	++e->file->warning;
	ft_dprintf(2, line_error(WARNING_LINE, e->tty2),
		e->file->name, line->y, error - line->s + 1);
	ft_dprintf(2, "redefinition of label '%.*s' not allowed",
		ft_strchr(error, LABEL_CHAR) - error, error);
	ft_dprintf(2, " (previous definition line %d)\n", y);
	err_pointer(e->tty2, line->s, error);
	ft_dprintf(2, "\n");
}

void	undefined_label(t_env *e, t_call *call, int *note, int tt)
{
	int len;

	++e->file->error;
	len = ft_strspn(call->s, LABEL_CHARS);
	ft_dprintf(2, line_error(ERR_LINE, e->tty2),
		e->file->name, call->line->y, call->s - call->line->s);
	ft_dprintf(2, "label '%.*s' is undefined ", len, call->s);
	if (tt)
		ft_dprintf(2, "(%d other%s", tt, tt > 1 ? "s)" : ")");
	ft_dprintf(2, "\n");
	err_pointer(e->tty2, call->line->s, call->s);
	ft_dprintf(2, "\n");
	if (!*note)
	{
		ft_dprintf(2, line_error(NOTE_LINE, e->tty2),
			e->file->name, call->line->y, call->s - call->line->s);
		ft_dprintf(2, "undefined label reported only once\n");
		*note = 1;
	}
	if (e->tty2)
		ft_dprintf(2, "{R}");
}
