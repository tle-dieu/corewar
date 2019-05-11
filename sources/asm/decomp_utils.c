/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decomp_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acompagn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:53:06 by acompagn          #+#    #+#             */
/*   Updated: 2019/05/11 16:52:22 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void			init_line(t_decomp *d)
{
	d->x = 0;
	d->op = d->content[d->i];
	nb_in_buff(d, d->i, 1);
	str_in_buff(d, ":\t\t"); // espaces au lieu de tab (macro TAB_SIZE) ?
	str_in_buff(d, g_op_tab[d->content[d->i++] - 1].label);
	d->buff_d->tab[d->y][d->x++] = ' ';
}

int				generate_decomp_file(t_env *e, t_decomp *d, t_buff_d *ptr)
{
	int			i;
	int			fd;
	char		*s;

	ptr = d->main_ptr;
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
		free_buff_decomp(d);
		alloc_error(e);
	}
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
	return (free_buff_decomp(d));
}

void			nb_in_buff(t_decomp *d, int nb, int padding)
{
	unsigned int	len;
	unsigned int	abs;
	unsigned int	tmp;

	len = (nb < 0) ? 2 : 1;
	abs = (nb < 0) ? -nb : nb;
	tmp = abs;
	while (tmp > 9)
	{
		len++;
		tmp /= 10;
	}
	tmp = d->x;
	d->x += padding ? 6 - 1 : len - 1;
	while (abs > 9)
	{
		d->buff_d->tab[d->y][d->x--] = (abs % 10) + 48;
		abs /= 10;
	}
	d->buff_d->tab[d->y][d->x] = (abs % 10) + 48;
	if (nb < 0)
		d->buff_d->tab[d->y][--d->x] = '-';
	while (d->x > (int)tmp && padding)
		d->buff_d->tab[d->y][--d->x] = '0';
	d->x = padding ? tmp + 6 : tmp + len;
}

void			str_in_buff(t_decomp *d, char *s)
{
	while (*s)
		d->buff_d->tab[d->y][d->x++] = *s++;
}

int				compute_param(t_decomp *d, int i, int size)
{
	int		nb;
	int		j;

	nb = 0;
	j = 0;
	while (size--)
	{
		nb *= 256;
		nb += d->content[i + j++];
	}
	return (nb);
}
