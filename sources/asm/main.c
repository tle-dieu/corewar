/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/29 20:50:22 by tle-dieu         ###   ########.fr       */
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

//protection file too long
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

#include <errno.h> //  a retirer

void	compile_write(t_env *e, unsigned char *bin)
{
	char	*s;
	int		fd;

	if (!e->actual->output)
	{
		if (!(s = ft_strrchr(e->actual->name, '.')))
		{
			if (!(e->actual->output = ft_strjoin(e->actual->name, ".cor")))
			{
				ft_printf("malloc error\n");
				exit (0); // alloc error
			}
		}
		else if (!(e->actual->output = ft_strnew(s - e->actual->name))
		|| !ft_memcpy(e->actual->output, e->actual->name, s - e->actual->name)
		|| !ft_memcpy(e->actual->output + (s - e->actual->name), ".cor", 5))
		{
			ft_printf("malloc error\n");
			exit(0); //alloc error
		}

	}
	if ((fd = open(e->actual->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) != -1)
	{
		write(fd, bin, e->actual->i + PROG_NAME_LENGTH + COMMENT_LENGTH + 16);
		ft_printf("Writing output program to %s\n", e->actual->output);
		close(fd);
	}
	else
	{
		ft_printf("error: %s: '%s'\n", strerror(errno), e->actual->output);
		exit(0); // errno
	}
}

void	end_error(t_env *e, unsigned char *bin)
{
	if (e->actual->i > CHAMP_MAX_SIZE)
		ft_printf("{#ff3333}error champ too long\n{R}");
	if (!(e->actual->complete & 1) && ++e->actual->warning)
		ft_dprintf(2, "{#ff3333}missing name{R}\n");
	if (!(e->actual->complete & 2) && ++e->actual->warning)
		ft_dprintf(2, "{#ff3333}missing comment{R}\n");
	if (e->actual->warning)
		ft_dprintf(2, "%d %s ", e->actual->warning, e->actual->warning > 1 ? "warnings" : "warning");
	if (e->actual->warning && e->actual->error)
		ft_dprintf(2, "and ");
	if (e->actual->error)
		ft_dprintf(2, "%d %s ", e->actual->error, e->actual->error > 1 ? "errors" : "error");
	if (e->actual->error || e->actual->warning)
		ft_dprintf(2, "generated.\n");
	if (!e->actual->error)
	{
		if (!e->actual->error && e->actual->options & (O_HEXA | O_BIN))
			print_bin(e, bin, e->actual->i + HEADER_SIZE);
		if (!e->actual->error && !(e->actual->options & (O_HEXA | O_BIN)))
			compile_write(e, bin);
	}
}
void	compile(t_env *e)
{
	unsigned char	*cp;
	unsigned char	bin[BIN_MAX_SIZE];
	int				i;

	ft_bzero(bin, BIN_MAX_SIZE);
	i = 4;
	cp = bin;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_bytecode(e, cp - 4);
	i = 4;
	while (i--)
		bin[PROG_NAME_LENGTH + 11 - i] = e->actual->i >> i * 8;
	end_error(e, bin);
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
