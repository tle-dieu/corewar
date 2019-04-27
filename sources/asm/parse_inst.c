/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/27 17:42:19 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"
#include <stdlib.h>

int		get_curr_inst(char *str)
{
	int i;

	i = 0;
	while (g_op_tab[i].label  
	&& (ft_strncmp(str, g_op_tab[i].label, g_op_tab[i].len) 
	|| (str[g_op_tab[i].len] != '\t' && str[g_op_tab[i].len] != ' ')))
		i++;
	if (!g_op_tab[i].label)
		return (42);	
	return (i + 1);
}


int		create_call(t_env *e, t_inst *inst, char *s, t_label *label, int i)
{
	t_label *new;
	t_call	*call;
	int		len;

	new = NULL;
	if (!label)
	{
		if (!(new = (t_label *)malloc(sizeof(t_label))))
			exit (0); // alloc error
		len = ft_strcspn(s, SPACES);
		label = new;
		new->index = -1;
		label->call = NULL;
		new->next = e->actual->label;
		e->actual->label = new;
		if (!(new->name = ft_strndup(s, len)))
		{
			free(new);
			exit(0); // alloc error
		}
	}
	if (!(call = (t_call *)malloc(sizeof(t_call))))
	{
		if (new)
			free(new->name); // remplacer par fonction free call ?
		free(new);
		exit(0);
	}
	e->actual->last->free = 0;
	call->line = e->actual->last;
	call->index_inst = e->actual->i;
	call->index_call = inst->index;
	call->s = s;
	call->size = inst->s[i]; //envoyer que inst et pas i
	call->next = label->call;
	label->call = call;
	return (1);
}

void	get_label_call(t_env *e, t_inst *inst, char *s, int i)
{
	int		len;
	t_label *label;

	label = e->actual->label;
	len = ft_strcspn(s, SPACES);
	ft_printf(MAGIC_C"str label: %s\n{R}", s);
	while (label)
	{
		if (!ft_strncmp(s, label->name, len) && ft_printf("YES\n\n") && !label->name[len])
		{
			if (label->index != -1)
			{
				inst->p[i] = label->index - e->actual->i;
				ft_printf(MAGIC_C"call: '%s' index: %d\n{R}", s, inst->p[i]);
				return ;
			}
			break ;
		}
		label = label->next;
	}
	if (!label || label->index == -1)
		create_call(e, inst, s, label, i); // separer en deux pour trop args
}

int is_direct(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	int i;
	char *to_free;

	(void)e;
	i = 0;
	tmp = str;
	if (*tmp == DIRECT_CHAR)
	{
		++tmp;
		to_free = ft_strcdup(tmp, *SEPARATOR_CHAR);
		inst->s[inst->index] = g_op_tab[inst->op - 1].dir_size ? 2 : 4;
		inst->t[inst->index] = 2;
		if (*tmp == LABEL_CHAR)
			get_label_call(e, inst, tmp + 1, i);
		else
			inst->p[inst->index] = ft_atoi(to_free);
		return (1);
	}
	return (0);
}

int is_indirect(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	int i;
	char *to_free;

	(void)e;
	i = 0;
	tmp = str;
	inst->s[inst->index] = IND_SIZE;
	inst->t[inst->index] = 3;
	to_free = ft_strcdup(tmp, *SEPARATOR_CHAR);
	if (*tmp == LABEL_CHAR)
		get_label_call(e, inst, tmp + 1, i);
	else
		inst->p[inst->index] = ft_atoi(to_free);
	free(to_free);
	return (1);
}

int		is_reg(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	int i;
	char *to_free;

	(void)e;
	i = 0;
	tmp = str;
	if (*tmp == 'r')
	{
		tmp++;
		inst->s[inst->index] = 1;
		inst->t[inst->index] = 1;
		to_free = ft_strcdup(tmp, *SEPARATOR_CHAR);
		inst->p[inst->index] = ft_atoi(to_free);
		free(to_free);
		return (1);
	}
	return (0);
}

void get_ocp(t_inst *inst)
{
	int  i;
	int ocp;
	const int tab[3] = {64, 16, 4};

	i = 0;
	ocp = 0;
	while (i < inst->nb_p)
	{
		ocp += tab[i] * inst->t[i];
		i++;
		ft_printf("\t%d\n", ocp);
	}
	inst->ocp = ocp;
}

int check_params(t_env *e, char *str, t_inst *inst)
{
	int i;
	char *tmp;

	tmp = str;
	i = 0;
	ft_printf("-------------------------------------------------------------------------------\n");
	while (*tmp)
	{
		inst->t[i] = 0;
		inst->index = i;
		tmp += ft_strspn(tmp, SPACES);
		ft_printf("0) %s\n", tmp);
		is_direct(e, tmp, inst) || is_reg(e, tmp, inst) || is_indirect(e, tmp, inst);
		ft_printf("1) %s\n", tmp);
		tmp += ft_strcspn(tmp, SPACES SEPARATOR_CHAR);
		ft_printf("2) %s\n", tmp);
		tmp += ft_strspn(tmp, SPACES);
		ft_printf("3) %s\n", tmp);
		++i;
		if (*tmp && *tmp != *SEPARATOR_CHAR)
		{
			ft_printf("{#ff3333}ERROR: illegal character %c after argument in %s\n{R}", *tmp, str);
			break ;
		}
		else if (!*tmp)
			break ;
		++tmp;
	}
	if (i > g_op_tab[inst->op - 1].nb_param)
		ft_printf("{#ff3333}ERROR: too many argument for instruction '%s', must have %d have %d\n{R}", g_op_tab[inst->op - 1].label, g_op_tab[inst->op - 1].nb_param, i);
	else if (i < g_op_tab[inst->op - 1].nb_param)
		ft_printf("{#ff3333}ERROR: argument missing for instruction '%s', must have %d have %d\n{R}", g_op_tab[inst->op - 1].label, g_op_tab[inst->op - 1].nb_param, i);
	return (1);
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

	i = 0;
	cp[e->actual->i++] = inst->op; //check champ max size ou check avant ? stopper ou continuer en faisant repartir index a 0
	if (g_op_tab[inst->op - 1].ocp)
		cp[e->actual->i++] = inst->ocp;
	while (i < inst->nb_p)
	{
		k = inst->s[i];
		while (k--)
			cp[e->actual->i++] = inst->p[i] >> k * 8;
		i++;
	}
}

t_inst	*parse_inst(t_env *e, char *str, unsigned char *cp)
{	
	t_inst inst;
	char *tmp;

	print_label(e);
	inst = (t_inst){.ocp = 0};
	ft_printf(NAME_C"\nparse_inst: {R}%s\n", str);
	if ((inst.op = get_curr_inst(str)) <= 16)
	{
		inst.nb_p = g_op_tab[inst.op - 1].nb_param;
		tmp = str + ft_strlen(g_op_tab[inst.op - 1].label);
		tmp += ft_strspn(tmp, SPACES);
		check_params(e, tmp, &inst);
		if (g_op_tab[inst.op - 1].ocp)
			get_ocp(&inst);
	}
	write_inst(e, &inst, cp);
	print_inst(&inst, str);
	return (NULL);
}
