/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/22 04:07:42 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h> // tmp
#include <unistd.h>

//verifier que asm passe seulement ' ' et '\t'

int		pass_line(char *s)
{
	int len;
	int i;

	len = sizeof(COMMENT_CHAR);
	(void)len;
	while (*s)
	{
		i = 0;
		while (i < len)
			if (*s == COMMENT_CHAR[i++])
				return (1);
		if (*s != '\t' && *s != ' ')
			return (0);
		s++;
	}
	return (1);
}
int		add_line(t_env *e, char **line)
{
	t_line	*new;
	int		ret;

	while ((ret = get_next_line(e->actual->fd, line)) >= -1)
	{
		if (ret <= 0)
			return (ret == -1 ? alloc_error(e) : 0);
		if (pass_line(*line))
		{
			ft_printf("line passee: |%s|\n", *line);
			free(*line);
			*line = NULL;
		}
		else
			break ;
	}
	if (!(new = (t_line *)malloc(sizeof(t_line))))
	{
		free(*line);
		alloc_error(e);
	}
	ft_printf(COMMENT_C"\nline: |%s|\n{R}", *line);
	new->next = NULL;
	new->s = *line;
	new->y = e->actual->line_nb++;
	if (!e->actual->begin)
		e->actual->begin = new;
	else
		e->actual->last->next = new;
	e->actual->last = new;
	return (1);
}

void	compile(t_env *e)
{
	unsigned char	*cp;
	unsigned char	bin[BIN_MAX_SIZE];
	int				i;

	ft_printf("{yellow}----------COMPILE----------\n{R}");
	ft_printf(STR_C"file: %s\n{R}", e->actual->name);
	ft_bzero(bin, BIN_MAX_SIZE);
	i = 4;
	cp = bin;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_header(e, cp);
	if (e->actual->complete & 1)
		ft_printf(STR_C"name:{R} |%s|\n", &bin[4]);
	if (e->actual->complete & 2)
		ft_printf(STR_C"comment:{R} |%s|\n\n", &bin[PROG_NAME_LENGTH + 12]);
	/* if (!e->actual->error) */
		/* print_bin(bin, BIN_MAX_SIZE); */
	if (e->actual->error)
		ft_dprintf(2, "%d %s generated\n", e->actual->error, e->actual->error > 1 ? "errors" : "error");
	ft_printf("\n"); // a retirer
}

int		main(int ac, char **av)
{
	t_env	e;

	e = (t_env){isatty(2), NULL, NULL, av[0]};
	if (ac < 2)
		return (usage(&e, 3));
	if (!parse_command_line(&e, ac, av))
		return (!e.file ? usage(&e, 3) : 1);
	ft_printf("color ? %s\n", e.tty ? "yes" : "no");
	print_files(e.file);
	e.actual = e.file;
	while (e.actual)
	{
		compile(&e);
		e.actual = e.actual->next;
	}
	free_lst_file(e.file);
}
