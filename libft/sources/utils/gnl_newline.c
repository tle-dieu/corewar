/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_newline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 16:37:36 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/16 20:46:07 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static int		free_fd(t_gnl **begin, t_gnl *del)
{
	t_gnl *prev;
	t_gnl *actual;

	prev = NULL;
	actual = *begin;
	while (actual != del)
	{
		prev = actual;
		actual = actual->next;
	}
	free(del->str);
	if (!prev)
		*begin = del->next;
	else
		prev->next = del->next;
	free(del);
	return (-1);
}

static t_gnl	*choose_fd(t_gnl **begin_list, int fd)
{
	t_gnl *list;
	t_gnl *new;

	list = *begin_list;
	while (list)
	{
		if (list->fd == fd)
			return (list);
		list = list->next;
	}
	if (!(new = (t_gnl*)malloc(sizeof(t_gnl))))
		return (NULL);
	if (!(new->str = ft_strnew(0)))
	{
		free(new);
		return (NULL);
	}
	new->fd = fd;
	new->len = 0;
	new->next = *begin_list;
	*begin_list = new;
	return (new);
}

static int		check_line(t_gnl *actual, int ret, char **line, char *next_line)
{
	char	*tmp;
	size_t	len;

	len = (next_line ? (size_t)(next_line - actual->str) : actual->len) + 1;
	if (ret == 0 && !actual->len)
		return (0);
	if (ret < 0 || !(*line = (char *)malloc(sizeof(char) * (len + 1)))
	|| !(ft_memcpy(*line, actual->str, len)))
		return (-1);
	(*line)[len] = '\0';
	if (next_line)
	{
		if (!(tmp = ft_memdup(next_line, actual->len - len)))
			return (-1);
		free(actual->str);
		actual->str = tmp;
	}
	actual->len -= len + (unsigned)(next_line != NULL);
	return (len + 1);
}

int				gnl_newline(const int fd, char **line)
{
	int				ret;
	char			buff[BS_GNL];
	static t_gnl	*list;
	t_gnl			*actual;
	char			*tmp;

	if (!(ret = 0) && (fd < 0 || !line || read(fd, buff, 0) < 0 || BS_GNL <= 0))
		return (-1);
	if (!(actual = choose_fd(&list, fd)))
		return (-1);
	while (!(tmp = ft_memchr(actual->str, '\n', actual->len)))
	{
		if ((ret = read(fd, buff, BS_GNL)) <= 0)
			break ;
		tmp = actual->str;
		if (!(actual->str = ft_memjoin(actual->str, buff, actual->len, ret)))
			return (free_fd(&list, actual));
		actual->len += ret;
		free(tmp);
		if (actual->len > MAX_SIZE_STATIC)
			return (!free_fd(&list, actual));
	}
	if ((ret = check_line(actual, ret, line, tmp)) <= 0)
		free_fd(&list, actual);
	return (ret);
}
