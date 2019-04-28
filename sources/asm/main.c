/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/28 05:08:58 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h> // tmp
#include <unistd.h>
#include <fcntl.h>

//verifier que SPACES est partout

int		pass_line(char *s)
{
	int		i;

	s += ft_strspn(s, SPACES);
	if (!*s || ft_strchr(COMMENT_CHAR, *s))
		return (1);
	if (*(s + (i = ft_strcspn(s, COMMENT_CHAR)))
	&& (!(ft_ncount_occ(s, '"', i) & 1) || !ft_strchr(s + i, '"')))
		*(s + i) = '\0';
	return (0);
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
	new->free = 1;
	ft_printf(COMMENT_C"\n|%s|\n{R}", *line);
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

//penser a close tous les fd

void	compile_write(t_env *e, unsigned char *bin)
{
	char	*s;
	int		fd;

	ft_printf("WRITE FILE\n");
	if (!e->actual->output)
	{
		if (!(s = ft_strrchr(e->actual->name, '.')))
		{
			if (!(e->actual->output = ft_strjoin(e->actual->name, ".cor")))
			{
				ft_printf("EXIT\n");
				exit (0); // alloc error
			}
		}
		else if (!(e->actual->output = ft_strnew(s - e->actual->name))
		|| !ft_memcpy(e->actual->output, e->actual->name, s - e->actual->name)
		|| !ft_memcpy(e->actual->output + (s - e->actual->name), ".cor", 5))
		{
			ft_printf("EXIT\n");
			exit(0); //alloc error
		}
	}
	ft_printf("output: %s\n", e->actual->output);
	if ((fd = open(e->actual->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) == -1)
	{
		ft_printf("EXIT\n");
		exit(0); // errno et return
	}
	write(fd, bin, e->actual->i + PROG_NAME_LENGTH + COMMENT_LENGTH + 16);
	close(fd);
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
	print_label(e);
	if (e->actual->complete & 1)
		ft_printf(STR_C"name:{R} |%s|\n", &bin[4]);
	if (e->actual->complete & 2)
		ft_printf(STR_C"comment:{R} |%s|\n\n", &bin[PROG_NAME_LENGTH + 12]);
	if (e->actual->error)
		ft_dprintf(2, "%d %s generated\n", e->actual->error, e->actual->error > 1 ? "errors" : "error");
	i = 4;
	while (i--)
		bin[PROG_NAME_LENGTH + 11 - i] = e->actual->i >> i * 8;
	if (!e->actual->error && e->actual->options & (O_HEXA | O_BIN))
		print_bin(e, bin, e->actual->i + PROG_NAME_LENGTH + COMMENT_LENGTH + 16);
	if (!e->actual->error && !(e->actual->options & (O_HEXA | O_BIN)))
		compile_write(e, bin);
	ft_printf("\n"); // a retirer
}

int		main(int ac, char **av)
{
	t_env	e;
	t_file	*next;

	e = (t_env){isatty(1), isatty(2), 0, NULL, NULL, av[0], NULL};
	if (ac < 2)
		return (usage(&e, 3));
	if (!parse_command_line(&e, ac, av))
		return (!e.file ? usage(&e, 3) : 1);
	if (!e.file)
		return (usage(&e, 3));
	print_files(e.file);
	e.actual = e.file;
	while (e.actual)
	{
		compile(&e);
		next = e.actual->next;
		free_line(e.actual);
		free(e.actual);
		e.actual = next;
	}
}
