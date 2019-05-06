/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:27:34 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/06 02:47:30 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include "op.h"
#include <stdlib.h> // tmp
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

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
		e->file->begin = new;
	else
	{
		if (e->file->last != e->file->begin)
			free_line(&e->file->last, 0);
		e->file->last = new;
	}
	e->file->last = new;
	*line = without_space ? without_space : *line;
	return (1);
}

//penser a close tous les fd

void	compile_write(t_env *e, unsigned char *header)
{
	char	*s;
	int		fd;

	if (!e->file->output)
	{
		if (!(s = ft_strrchr(e->file->name, '.')))
		{
			if (!(e->file->output = ft_strjoin(e->file->name, ".cor")))
				alloc_error(e);
		}
		else
		{
			if (!(e->file->output = ft_strnew(s - e->file->name + 4))
					|| !ft_memcpy(e->file->output, e->file->name, s - e->file->name)
					|| !ft_memcpy(e->file->output + (s - e->file->name), ".cor", 5))
				alloc_error(e);
		}
	}
	if ((fd = open(e->file->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)) != -1)
	{
		write(fd, header, HEADER_SIZE);
		e->file->buff = e->file->begin_buff;
		while (e->file->buff)
		{
			write(fd, e->file->buff->s, e->file->buff->len);
			e->file->buff = e->file->buff->next;
		}
		ft_printf("Writing output program to %s\n", e->file->output);
		close(fd);
	}
	else
	{
		ft_dprintf(2, line_error(ERR_ARGS, e->tty2), e->exname);
		ft_dprintf(2, "%s: '%s'\n", strerror(errno), e->file->output);
	}
	
}

void	missing_cmd(t_env *e, unsigned char *header, int cmd)
{
	char	*scmd;
	char	*default_str;
	int		len;

	++e->file->warning;
	if (cmd == NAME_CMD)
		len = sizeof(DEFAULT_NAME);
	else
		len = sizeof(DEFAULT_COMMENT);
	default_str = (cmd == NAME_CMD ? DEFAULT_NAME : DEFAULT_COMMENT);
	scmd = (cmd == NAME_CMD ? NAME_CMD_STRING : COMMENT_CMD_STRING);
	ft_dprintf(2, line_error(WARNING_FILE, e->tty2), e->file->name);
	ft_dprintf(2, "'%s' is undefined (set to '%s')\n", scmd, default_str); // changer erreur ?
	if (e->tty2)
		ft_dprintf(2, "{R}");
	ft_memcpy(header, default_str, len);
}

void    check_label_call(t_env *e)
{
	t_label *label;
	t_call  *call;
	int		len;
	int		note;
	int		tt;

	label = e->file->label;
	note = 0;
	while (label && e->file->error < MAX_ERROR)
	{
		if (label->index == -1)
		{
			tt = 0;
			call = label->call;
			while (call->next)
			{
				++tt;
				call = call->next;
			}
			++e->file->error;
			len = ft_strspn(call->s, LABEL_CHARS);
			ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, call->line->y, call->s - call->line->s);
			ft_dprintf(2, "label '%.*s' is undefined ", len, call->s); // revoir erreur
			if (tt)
				ft_dprintf(2, "(%d other%s", tt, tt > 1 ? "s)" : ")"); // revoir erreur
			ft_dprintf(2, "\n");
			err_pointer(e->tty2, call->line->s, call->s, 0);
			ft_printf("\n");
			if (!note)
			{
				ft_dprintf(2, line_error(NOTE_LINE, e->tty2), e->file->name, call->line->y, call->s - call->line->s);
				ft_dprintf(2, "undefined label reported only once\n");
				note = 1;
			}
			if (e->tty2)
				ft_dprintf(2, "{R}");
		}
		label = label->next;
	}
}

//checker pour tous les err_file que la couleur est reset
void	end_error(t_env *e, unsigned char *header)
{
	if (e->file->error < MAX_ERROR && e->file->i > CHAMP_MAX_SIZE)
	{
		++e->file->warning;
		ft_dprintf(2, line_error(WARNING_FILE, e->tty2), e->file->name); //fct
		ft_dprintf(2, "bytecode of the champion too big for the vm (%d for %d bytes)\n", e->file->i, CHAMP_MAX_SIZE);
		if (e->tty2)
			ft_dprintf(2, "{R}");
	}
	if (e->file->error < MAX_ERROR && !(e->file->complete & NAME_CMD))
		missing_cmd(e, header + 4, NAME_CMD);
	if (e->file->error < MAX_ERROR && !(e->file->complete & COMMENT_CMD))
		missing_cmd(e, header + PROG_NAME_LENGTH + 12, COMMENT_CMD);
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
	while (e.file)
	{
		compile(&e);
		next = e.file->next;
		free_file(&e.file);
		e.file = next;
	}
}
