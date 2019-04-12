/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/12 19:26:36 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <unistd.h>

#include <stdlib.h> // tmp

//error a gerer
//verifier que asm passe seulement ' ' et '\t'

void	print_synth(char *s, char replace)
{
	char	buff[100];
	int		i;
	int		j;

	while (*s)
	{
		i = 0;
		while (i + TAB_SIZE < 100 && *s)
		{
			if (*s == '\t')
			{
				j = 0;
				while (j++ < TAB_SIZE)
					buff[i++] = replace ? replace : ' ';
			}
			else
				buff[i++] = !replace ? *s : ' ';
			s++;
		}
		write(2, buff, i);
	}
	write(2, "\n", 1);
}

void	print_pointer(char *s, char *end)
{
	print_synth(s, 0);
	ft_ncount_occ(s, '\t', end - s);
	ft_printf("\x1b[%dC", end - s);
	ft_printf(GREEN_CURS"%c{R}", '^');
}

void	print_wave(char *s)
{
	int i;

	i = 0;
	ft_printf(GREEN_CURS);
	while (*s)
	{
		i = *s == '\t' ? TAB_SIZE : 1;
		while (i--)
			write(1, "~", 1);
		s++;
	}
	ft_printf("{R}");
}

int		error_header(t_file *file, int error, char *extra)
{
	ft_printf(FT_C"error_header\n{R}");
	file->error = 1;
	if (error == 1)
	{
		ft_printf("{bold}%s:%d:%d: "RED_ERR"error: {R}{bold}unexpected expression after .name declaration{R}\n", file->name, file->last->y, extra - file->last->s + 1);
		print_pointer(file->last->s, extra);
	}
	else if (error == 2)
	{
		ft_printf("{bold}%s:%d:%d: "RED_ERR"error: {R}{bold}.name declaration too long{R}\n", file->name, file->begin->y, extra - file->begin->s + 1);
		print_pointer(file->begin->s, extra++);
		print_wave(extra);
	}
	write(1, "\n", 1);
	return (0);
}

char	*check_end_str(char **end)
{
	char *s;

	s = *end + 1;
	ft_printf("check: %s\n", s);
	while (*s == ' ' || *s == '\t')
		s++;
	if (!*s)
		return (NULL);
	else
	{
		ft_printf("%hhd\n", *s);
		*end = s;
		return (s);
	}
}

int		add_line(char **line, t_file *file)
{
	t_line	*new;

	*line = NULL;
	if (get_next_line(file->fd, line) != 1)
		return (0);
	if (!(new = (t_line *)malloc(sizeof(t_line))))
	{
		free(*line);
		return (0);
	}
	new->next = NULL;
	new->s = *line;
	if (!file->begin)
	{
		new->y = 1;
		file->begin = new;
	}
	else
	{
		new->y = file->last->y + 1;
		file->last->next = new;
	}
	file->last = new;
	return (1);
}

int		multi_line(t_file *file, char *buff, int max_size, int i)
{
	char *line;
	char *s;
	int	end;

	end = -1;
	ft_printf(FT_C"multi_line{R}\n");
	while (end == -1)
	{
		if (add_line(&line, file) != 1)
			return (0);
		s = line;
		while (*s && *s != '"')
		{
			if (i >= max_size)
				return (error_header(file, 2, ft_strchr(file->begin->s, '"')));
			buff[i++] = *s++;
		}
		if (*s == '"')
			end = !check_end_str(&s);
	}
	buff[i] = '\0';
	return (error_header(file, !end, s));
}

//return ou exit mais envoie de line dans ce cas pour free
//peut etre meilleur moyen de gerer les erreurs
int		get_name(t_file *file, char *s, unsigned char *cp)
{
	char	buff[PROG_NAME_LENGTH + 1];
	int		i;
	char	*t;

	(void)cp;
	ft_printf(FT_C"get_name | line: %s\n{R}", s);
	i = 0;
	if (!(t = ft_strchr(s, '"')))
	{
		ft_printf(RED_ERR"no string after name\n{R}"); //fontion err
		return (error_header(file, 3, s));
	}
	s = t;
	while (*++s && *s != '"')
	{
		if (i >= PROG_NAME_LENGTH)
		{
			ft_printf(RED_ERR"name too long\n{R}"); //fonction err
			return (0);
		}
		buff[i++] = *s;
	}
	if (!*s)
	{
		if (!(multi_line(file, buff, PROG_NAME_LENGTH, i)))
			return (0);
	}
	else
		buff[i] = '\0';
	if (check_end_str(&s))
	{
		ft_printf(RED_ERR"char after name\n{R}"); // fonction err
		return (0);
	}
	return (1);
}

int		get_header(t_file *file, unsigned char *cp)
{
	char	*line;
	int		i;

	(void)cp;
	line = NULL;
	ft_printf(FT_C"get_header\n{R}");
	while (add_line(&line, file) == 1)
	{
		i = 0;
		ft_printf("new line\n");
		while (line[i])
		{
			if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			{
				if (!ft_strncmp(line + i, NAME_CMD_STRING, sizeof(NAME_CMD_STRING) - 1))
				{
					if (!(get_name(file, line + i + sizeof(NAME_CMD_STRING), cp)))
						exit(0);
					break ;
				}
				else if (!ft_strncmp(COMMENT_CMD_STRING, line + i, sizeof(COMMENT_CMD_STRING) - 1))
					ft_printf("COMMENT: %s\n", line + i);
				else
					return (0);
			}
			i++;
		}
		line = NULL;
	}
	return (1);
}

void	compile(t_file *file)
{
	unsigned char	*cp;
	unsigned char	bin[BIN_MAX_SIZE];
	int				i;

	ft_bzero(bin, BIN_MAX_SIZE);
	i = 4;
	cp = bin;
	while (i--)
		*cp++ = COREWAR_EXEC_MAGIC >> i * 8;
	get_header(file, cp);
	print_bin(bin, cp - bin);
}

int		main(int ac, char **av)
{
	t_file	*file;
	t_file	*actual;

	if (ac < 2)
		return (usage(av[0], 0));
	if (!(file = parse_command_line(ac, av)))
		return (1);
	print_files(file);
	actual = file;
	while (actual)
	{
		compile(actual);
		actual = actual->next;
	}
	free_lst_file(file);
}
