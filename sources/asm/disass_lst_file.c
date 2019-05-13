/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disass_lst_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 13:04:44 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/13 13:22:01 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int			free_buff_disass(t_disass *d)
{
	t_buff_d	*ptr;
	t_buff_d	*tmp;

	ptr = d->main_ptr;
	free(d->content);
	d->content = NULL;
	d->main_ptr = NULL;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return (0);
}

int			add_buff_link(t_env *e, t_disass *d)
{
	t_buff_d	*new;
	t_buff_d	*ptr;
	int			i;

	i = -1;
	d->y = 0;
	if (!(new = (t_buff_d*)malloc(sizeof(t_buff_d))))
	{
		free_buff_disass(d);
		alloc_error(e);
	}
	new->next = NULL;
	while (++i < BS_DECOMP)
		ft_bzero(new->tab[i], COMMENT_LENGTH + 11);
	if (!d->buff_d && (d->main_ptr = new))
	{
		d->buff_d = new;
		return (1);
	}
	ptr = d->buff_d;
	while (ptr && ptr->next)
		ptr = ptr->next;
	ptr->next = new;
	d->buff_d = ptr->next;
	return (1);
}

static void	generate_file_name(t_env *e, t_disass *d)
{
	char	*s;

	if (!(s = ft_strrchr(e->file->name, '.')))
		e->file->output = ft_strjoin(e->file->name, "_disass.s");
	else
	{
		(e->file->output = ft_strnew(s - e->file->name + 9))
			&& ft_memcpy(e->file->output, e->file->name, s - e->file->name)
			&& ft_memcpy(e->file->output + (s - e->file->name), "_disass.s", 9);
	}
	if (!e->file->output)
	{
		free_buff_disass(d);
		alloc_error(e);
	}
}

int			generate_disass_file(t_env *e, t_disass *d, t_buff_d *ptr)
{
	int			i;
	int			fd;

	ptr = d->main_ptr;
	generate_file_name(e, d);
	fd = open(e->file->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd != -1)
	{
		while (ptr && (i = -1))
		{
			while (*ptr->tab[++i])
				ft_dprintf(fd, "%s\n", ptr->tab[i]);
			ptr = ptr->next;
		}
		ft_printf("Writing output assembly to %s\n", e->file->output);
		close(fd);
	}
	else
	{
		ft_dprintf(2, line_error(ERR_ARGS, e->tty2), e->exname);
		ft_dprintf(2, "%s: '%s'\n", strerror(errno), e->file->output);
	}
	return (free_buff_disass(d));
}

int			split_champ(t_env *e, t_disass *d, unsigned char *line, ssize_t ret)
{
	int		i;
	int		k;

	i = 3;
	k = 0;
	if (!(d->content = (unsigned char *)malloc(sizeof(unsigned char)
					* (d->size + 2))))
		alloc_error(e);
	ft_bzero(d->content, d->size + 2);
	while (i++ < NAME_COMM_SIZE + 8)
	{
		if (i == PROG_NAME_LENGTH + 8)
			k = 0;
		if (i < PROG_NAME_LENGTH + 8)
			d->name[k++] = line[i];
		else if (i > PROG_NAME_LENGTH + 11)
			d->comment[k++] = line[i];
	}
	if ((ret = read(e->file->fd, d->content, d->size + 1)) == -1
			|| ret != d->size)
	{
		return (disass_error(e,
					ret == -1 ? NULL : "Champion size does not match", d));
	}
	return (!d->content[d->size]);
}
