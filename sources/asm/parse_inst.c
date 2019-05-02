/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/02 14:03:41 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"
#include <stdlib.h>

size_t	ft_strrspn(const char *s, const char *accept)
{
	const char	*tmp;
	size_t		i;

	i = ft_strlen(s) - 1;
	while (i > 0)
	{
		tmp = accept;
		while (*tmp != s[i])
			if (!*tmp++)
				return (i);
		i++;
	}
	return (i);
}

int		get_curr_inst(char *str)
{
	int	i;

	i = 0;
	while (g_op_tab[i].label && (ft_strncmp(str, g_op_tab[i].label, g_op_tab[i].len) 
			|| (str[g_op_tab[i].len] != '\t' && str[g_op_tab[i].len] != ' ')))
		i++;
	if (!g_op_tab[i].label)
		return (42);	
	return (i + 1);
}

void get_ocp(t_inst *inst)
{
	int			i;
	int			ocp;
	const int	tab[3] = {64, 16, 4};///decalage binaire

	i = 0;
	ocp = 0;
	while (i < inst->nb_p)
	{
		ocp += tab[i] * inst->t[i];
		i++;
	}
	inst->ocp = ocp;
}

void	print_inst(t_inst *inst, char *str)
{
	int i;

	i = 0;
	ft_printf("\nstr\t= %s\n", str);
	ft_printf("op\t= %d\n", inst->op);
	ft_printf("ocp\t= %d\n", inst->ocp);
	ft_printf("nb_p\t= %d\n", inst->nb_p);
	ft_printf("params:\n");
	while (i < inst->nb_p)
	{
		ft_printf("\t p[%d] = %d", i, inst->p[i]);
		ft_printf("\t s[%d] = %d", i, inst->s[i]);
		ft_printf("\t t[%d] = %d\n", i, inst->t[i]);
		i++;
	}
}

void	write_inst(t_env *e, t_inst *inst, unsigned char *cp)
{
	int i;
	int	k;
	int j;

	i = 0;
	if (inst->index > CHAMP_MAX_SIZE && !e->file->too_long) //fonction error
	{
		e->file->too_long = 1;
		++e->file->error;
		e->file->i = inst->index;
	}
	if (!e->file->too_long && !inst->error && !e->file->error)
	{
		j = 0;
		cp[e->file->i + j++] = inst->op;
		if (g_op_tab[inst->op - 1].ocp)
			cp[e->file->i + j++] = inst->ocp;
		while (i < inst->nb_p)
		{
			k = inst->s[i];
			while (k--)
				cp[e->file->i + j++] = inst->p[i] >> k * 8;
			i++;
		}
	}
	if (!e->file->too_long && !inst->error)
		e->file->i = inst->index;
}

t_inst	*parse_inst(t_env *e, char *str, unsigned char *cp)
{	
	t_inst	inst;
	char	*tmp;

	inst = (t_inst){.ocp = 0}; // verifier norme
	if ((inst.op = get_curr_inst(str)) <= 16)
	{
		inst.nb_p = g_op_tab[inst.op - 1].nb_param;
		tmp = str + ft_strlen(g_op_tab[inst.op - 1].label);
		tmp += ft_strspn(tmp, SPACES);
		check_params(e, tmp, &inst);
		if (g_op_tab[inst.op - 1].ocp)
			get_ocp(&inst);
	}
	else
		error_unknow_inst(e, str);
	write_inst(e, &inst, cp);
	return (NULL);
}
