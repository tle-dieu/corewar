/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/08 22:35:20 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <unistd.h>

#include <stdlib.h> // tmp

//error a gerer

char *check_end_str(char *s)
{
	ft_printf("check: %s\n", s);
	while (*s == ' ' || *s == '\t')
		s++;
	if (!*s)
		return (NULL);
	else
	{
		ft_printf("%hhd\n", *s);
		return (s);
	}
}
int		multi_line(int fd, char *buff, int max_size, int i)
{
	char *line;
	char *s;
	int	end;

	end = -1;
	while (end == -1)
	{
		if (get_next_line(fd, &line) != 1)
			return (0);
		s = line;
		while (*s && *s != '"')
		{
			if (i >= max_size)
			{
				free(line);
				ft_printf(RED_ERR"name too long\n{reset}"); //fonction err
				return (0);
			}
			buff[i++] = *s++;
		}
		if (*s == '"')
			end = !check_end_str(s + 1);
		free(line);
	}
	buff[i] = '\0';
	if (!end)
		ft_printf(RED_ERR"char after name\n{reset}"); // fonction err
	return (end);
}

//return ou exit mais envoie de line dans ce cas pour free
//peut etre meilleur moyen de gerer les erreurs
int		get_name(int fd, char *s, unsigned char *cp)
{
	char	buff[PROG_NAME_LENGTH + 1];
	int		i;

	(void)cp;
	ft_printf("get name: %s\n", s);
	i = 0;
	if (!(s = ft_strchr(s, '"')))
	{
		ft_printf(RED_ERR"no string after name\n{reset}"); //fontion err
		return (0);
	}
	while (*++s && *s != '"')
	{
		if (i >= PROG_NAME_LENGTH)
		{
			ft_printf(RED_ERR"name too long\n{reset}"); //fonction err
			return (0);
		}
		buff[i++] = *s;
	}
	if (!*s)
	{
		if (!(multi_line(fd, buff, PROG_NAME_LENGTH, i)))
			return (0);
	}
	else
		buff[i] = '\0';
	if (check_end_str(s + 1))
	{
		ft_printf(RED_ERR"char after name\n{reset}"); // fonction err
		return (0);
	}
	return (1);
}

int		get_header(int fd, unsigned char *cp)
{
	char	*line;
	int		i;

	(void)cp;
	line = NULL;
	while (get_next_line(fd, &line) == 1)
	{
		i = 0;
		ft_printf("line: %s\n", line);
		while (line[i])
		{
			if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			{
				ft_printf("line: %s\n", line + i);
				if (!ft_strncmp(line + i, NAME_CMD_STRING, sizeof(NAME_CMD_STRING) - 1))
				{
					if (!(get_name(fd, line + i + sizeof(NAME_CMD_STRING), cp)))
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
	get_header(file->fd, cp);
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
