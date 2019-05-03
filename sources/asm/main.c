/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/03 18:12:17 by matleroy         ###   ########.fr       */
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
			e->file->line_nb++;
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
		e->file->begin = new;
		if (PRINT)
			ft_printf("{cyan}add e->file->begin {R}%p\n", e->file->begin);
	}
	else
	{
		if (PRINT)
			ft_printf("{yellow}in add line ::\n{R}");
		if (e->file->last != e->file->begin)
		{
			if (PRINT)
				ft_printf("{cyan}before e->file->last {R}%p\n", e->file->last);
			free_line(&e->file->last);
			if (PRINT)
				ft_printf("{bold}{red}after e->file->last {R}%p\n", e->file->last);
		}
		e->file->last = new;
	}
	e->file->last = new;
	*line = without_space ? without_space : *line;
	if (PRINT)
		ft_printf("{purple}line: {R}'%s'\n", *line);
	return (1);
}

//penser a close tous les fd

#include <errno.h> //  a retirer

void	compile_write(t_env *e, unsigned char *header)
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
		else
		{
			if (!(e->file->output = ft_strnew(s - e->file->name + 4))
					|| !ft_memcpy(e->file->output, e->file->name, s - e->file->name)
					|| !ft_memcpy(e->file->output + (s - e->file->name), ".cor", 5))
			{
				ft_printf("malloc error\n");
				exit(0); //alloc error
			}
			if (PRINT)
				ft_printf("output: %s\n", e->file->output);
		}
	}
	if ((fd = open(e->file->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) != -1)
	{
		write(fd, header, HEADER_SIZE);
		e->file->buff = e->file->begin_buff;
		while (e->file->buff)
		{
			if (PRINT)
				ft_printf("buff: %p buff->next: %p\n", e->file->buff, e->file->buff->next);
			write(fd, e->file->buff->s, e->file->buff->len);
			e->file->buff = e->file->buff->next;
		}
		ft_printf("Writing output program to %s\n", e->file->output);
		close(fd);
	}
	else
	{
		ft_printf("error: %s: '%s'\n", strerror(errno), e->file->output);
		exit(0); // errno
	}
}

void	end_error(t_env *e, unsigned char *header)
{
	if (e->file->i > CHAMP_MAX_SIZE)
		ft_printf("{#ff3333}warning champ too long\n{R}");
	if (!(e->file->complete & COMMENT_CMD) && ++e->file->warning)
		ft_dprintf(2, "{#ff3333}warning missing name{R}\n");
	if (!(e->file->complete & NAME_CMD) && ++e->file->warning)
		ft_dprintf(2, "{#ff3333}warning missing comment{R}\n");
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
			print_bin(e, header);
		if (!e->file->error && !(e->file->options & (O_HEXA | O_BIN)))
			compile_write(e, header);
	}
}

void	compile(t_env *e)
{
	unsigned char	*cp;
	unsigned char	header[HEADER_SIZE];
	int				i;

	ft_bzero(header, HEADER_SIZE);
	i = 4;
	cp = header;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_bytecode(e, header);
	i = 4;
	while (i--)
		header[PROG_NAME_LENGTH + 11 - i] = e->file->i >> i * 8;
	end_error(e, header);
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
	if (PRINT)
		print_files(e.file);
	while (e.file)
	{
		ft_printf("compile file: %s\n", e.file->name);
		if (PRINT )
		{
			print_entire_file(&e);
		}
		compile(&e);
		next = e.file->next;
		if (PRINT)
		{
			ft_printf("free file %p\n", e.file);
			ft_printf("fin free file %p\n", e.file);
		}
		free_file(&e.file);
		e.file = next;
	}
}
