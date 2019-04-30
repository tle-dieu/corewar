/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/30 11:08:34 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h> // tmp
#include <unistd.h>
#include <fcntl.h>

//verifier que SPACES est partout

char	*pass_line(char *s)
{
	int		i;

	s += ft_strspn(s, SPACES);
	if (!*s || ft_strchr(COMMENT_CHAR, *s))
		return (NULL);
	if (*(s + (i = ft_strcspn(s, COMMENT_CHAR)))
			&& (!(ft_ncount_occ(s, '"', i) & 1) || !ft_strchr(s + i, '"')))
		*(s + i) = '\0';
	return (s);
}

//protection file too long
int		add_line(t_env *e, char **line)
{
	t_line	*new;
	int		ret;
	char	*without_space;

	without_space = NULL;
	*line = NULL;
	while ((ret = get_next_line(e->file->fd, line)) > 0)
	{
		if (!e->file->begin && !(without_space = pass_line(*line)))
		{
			free(*line);
			*line = NULL;
		}
		else
			break ;
	}
	if (ret <= 0)
		return (ret == -1 ? alloc_error(e) : 0);
	if (!(new = (t_line *)malloc(sizeof(t_line))))
	{
		free(*line);
		alloc_error(e);
	}
	new->free = 1;
	new->s = *line;
	new->y = e->file->line_nb++;
	if (!e->file->begin)
	{
		e->file->uniq_line = 1;
		e->file->begin = new;
	}
	else
	{
		if (!e->file->uniq_line)
			free_line(&e->file->last);
		e->file->uniq_line = 0;
		e->file->last = new;
	}
	e->file->last = new;
	*line = without_space ? without_space : *line;
	return (1);
}

//penser a close tous les fd

#include <errno.h> //  a retirer

void	compile_write(t_env *e, unsigned char *bin)
{
	char	*s;
	int		fd;

	if (!e->file->output)
	{
		if (!(s = ft_strrchr(e->file->name, '.')))
		{
			if (!(e->file->output = ft_strjoin(e->file->name, ".cor")))
			{
				ft_printf("malloc error\n");
				exit (0); // alloc error
			}
		}
		else if (!(e->file->output = ft_strnew(s - e->file->name))
				|| !ft_memcpy(e->file->output, e->file->name, s - e->file->name)
				|| !ft_memcpy(e->file->output + (s - e->file->name), ".cor", 5))
		{
			ft_printf("malloc error\n");
			exit(0); //alloc error
		}

	}
	if ((fd = open(e->file->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) != -1)
	{
		write(fd, bin, e->file->i + PROG_NAME_LENGTH + COMMENT_LENGTH + 16);
		ft_printf("Writing output program to %s\n", e->file->output);
		close(fd);
	}
	else
	{
		ft_printf("error: %s: '%s'\n", strerror(errno), e->file->output);
		exit(0); // errno
	}
}

void	end_error(t_env *e, unsigned char *bin)
{
	if (e->file->error < MAX_ERROR)
	{
		if (e->file->too_long)
			ft_printf("{#ff3333}error champ too long\n{R}");
		if (!(e->file->complete & 1) && ++e->file->warning)
			ft_dprintf(2, "{#ff3333}missing name{R}\n");
		if (!(e->file->complete & 2) && ++e->file->warning)
			ft_dprintf(2, "{#ff3333}missing comment{R}\n");
	}
	if (e->file->warning)
		ft_dprintf(2, "%d %s ", e->file->warning, e->file->warning > 1 ? "warnings" : "warning");
	if (e->file->warning && e->file->error)
		ft_dprintf(2, "and ");
	if (e->file->error)
		ft_dprintf(2, "%d %s ", e->file->error, e->file->error > 1 ? "errors" : "error");
	if (e->file->error || e->file->warning)
		ft_dprintf(2, "generated.\n");
	if (!e->file->error)
	{
		if (!e->file->error && e->file->options & (O_HEXA | O_BIN))
			print_bin(e, bin, e->file->i + HEADER_SIZE);
		if (!e->file->error && !(e->file->options & (O_HEXA | O_BIN)))
			compile_write(e, bin);
	}
}

void	compile(t_env *e)
{
	unsigned char	*cp;
	unsigned char	bin[BIN_MAX_SIZE + 1];
	int				i;

	ft_bzero(bin, BIN_MAX_SIZE);
	i = 4;
	cp = bin;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_bytecode(e, cp - 4);
	i = 4;
	while (i--)
		bin[PROG_NAME_LENGTH + 11 - i] = e->file->i >> i * 8;
	end_error(e, bin);
}

void	print_entire_file(t_env *e)
{
	ft_printf("{yellow}---------- T_ENV ----------{R}\n");
	ft_printf("\t->tty1: %d\n", e->tty1);
	ft_printf("\t->tty2: %d\n", e->tty2);
	ft_printf("\t->i: %d\n", e->i);
	ft_printf("\t->actual: %p\n", e->actual);
	ft_printf("\t->file: %p\n", e->file);
	ft_printf("\t->exname: %s\n", e->exname);
	ft_printf("\t->output: %s\n", e->output);
	ft_printf("{cyan}----------- FILE ----------{R}\n");
	ft_printf("\t->complete: %d\n", e->file->complete);
	ft_printf("\t->output: %s\n", e->file->output);
	ft_printf("\t->name: %s\n", e->file->name);
	ft_printf("\t->fd: %d\n", e->file->fd);
	ft_printf("\t->i: %d\n", e->file->i);
	ft_printf("\t->options: %u\n", e->file->options);
	ft_printf("\t->error: %d\n", e->file->error);
	ft_printf("\t->warning: %d\n", e->file->warning);
	ft_printf("\t->line_nb: %d\n", e->file->line_nb);
	ft_printf("\t->too_long: %d\n", e->file->too_long);
	ft_printf("\t->begin: %p\n", e->file->begin);
	ft_printf("\t->last: %p\n", e->file->last);
	ft_printf("\t->label: %p\n", e->file->label);
	ft_printf("\t->next: %p\n", e->file->next);
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
	e.actual = NULL;
	if (!e.file)
		return (usage(&e, 3));
	print_files(e.file);
	while (e.file)
	{
		ft_printf("compile file: %s\n", e.file->name);
		print_entire_file(&e);
		ft_printf("A\n");
		compile(&e);
		ft_printf("B\n");
		next = e.file->next;
		ft_printf("next: %p\n", e.file->next);
		ft_printf("free file %p\n", e.file);
		free_file(&e.file);
		ft_printf("C\n");
		e.file = next;
	}
}
