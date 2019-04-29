/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/28 18:32:43 by matleroy         ###   ########.fr       */
/*   Updated: 2019/04/27 19:26:10 by tle-dieu         ###   ########.fr       */
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

	new = NULL;
	if (!label)
	{
		if (!(new = (t_label *)malloc(sizeof(t_label))))
			exit (0); // alloc error
		label = new;
		new->index = -1;
		label->call = NULL;
		new->next = e->actual->label;
		e->actual->label = new;
		if (!(new->name = ft_strndup(s, e->i)))
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
	len = ft_strcspn(s, SPACES",");
	e->i = len;
	while (label)
	{
		if (!ft_strncmp(s, label->name, len) && !label->name[len])
		{
			if (label->index != -1)
			{
				inst->p[i] = label->index - e->actual->i;
				return ;
			}
			break ;
		}
		label = label->next;
	}
	if (!label || label->index == -1)
		create_call(e, inst, s, label, i); // separer en deux pour trop args
}

int		is_a_number(char *str)
{
	char *tmp;

	tmp = str + (*str == '-');
	while (ft_isdigit(*tmp))
		tmp++;
	tmp += ft_strspn(tmp, SPACES);
	if (*tmp)
		return (0);
	return (1);
}

int is_direct(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	char *to_free;

	tmp = str;
	if (*tmp == DIRECT_CHAR)
	{
		++tmp;
		inst->s[inst->i] = g_op_tab[inst->op - 1].dir_size ? 2 : 4;
		inst->t[inst->i] = 2;
		if (*tmp == LABEL_CHAR)
			get_label_call(e, inst, tmp + 1, inst->i);
		else
		{
			to_free = ft_strcdup(tmp, *SEPARATOR_CHAR);
			if (is_a_number(to_free))
				inst->p[inst->i] = ft_atoi(to_free);
			else
				ft_printf("{#ff3333}ERROR: bad direct '%s'\n", tmp);
			free(to_free);
		}
		return (1);
	}
	return (0);
}

int is_indirect(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	char *to_free;

	tmp = str;
	inst->s[inst->i] = IND_SIZE;
	inst->t[inst->i] = 3;
	if (*tmp == LABEL_CHAR)
		get_label_call(e, inst, tmp + 1, inst->i);
	else
	{
		to_free = ft_strcdup(tmp, *SEPARATOR_CHAR);
		if (is_a_number(to_free))
			inst->p[inst->i] = ft_atoi(to_free);
		else
		{
			ft_printf("{#ff3333}ERROR: bad direct '%s'\n", tmp);
			return (0);
		}
		free(to_free);
	}
	return (1);
}


int		is_reg(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	char *to_free;

	tmp = str;
	(void)e;
	if (*tmp == 'r')
	{
		tmp++;
		inst->s[inst->i] = 1;
		inst->t[inst->i] = 1;
		to_free = ft_strcdup(tmp, *SEPARATOR_CHAR);
		inst->p[inst->i] = ft_atoi(to_free);
		if (!ft_isdigit(*tmp))
			ft_printf("{#ff3333}Error: illegal character for register %c\n", *tmp);
		if (inst->p[inst->i] < 0 || inst->p[inst->i] > 16)
			ft_printf("{#ff3333}Error: try to acces to register[%d], register index must be between 1 and 16\n", inst->p[inst->i]);
		free(to_free);
		return (1);
	}
	return (0);
}

void get_ocp(t_inst *inst)
{
	int			i;
	int			ocp;
	const int	tab[3] = {64, 16, 4};

	i = 0;
	ocp = 0;
	while (i < inst->nb_p)
	{
		ocp += tab[i] * inst->t[i];
		i++;
	}
	inst->ocp = ocp;
}

int check_params(t_env *e, char *str, t_inst *inst)
{
	char *tmp;

	tmp = str;
	inst->index = e->actual->i + g_op_tab[inst->op - 1].ocp + 1;
	while (*tmp)
	{
		tmp += ft_strspn(tmp, SPACES);
		is_direct(e, tmp, inst) || is_reg(e, tmp, inst) || is_indirect(e, tmp, inst);
		tmp += ft_strspn(tmp, SPACES);
		tmp += ft_strcspn(tmp, SEPARATOR_CHAR);
		if (!(inst->t[inst->i] & g_op_tab[inst->op - 1].param[inst->i]))
		{
			ft_printf("inst->[inst->i] = %d :: %d", inst->t[inst->i], g_op_tab[inst->op - 1].param[inst->i]);
			ft_printf("{#ff3333}Error: bad type\n");
		}
		inst->index += inst->s[inst->i++];
		if (*tmp && *tmp != *SEPARATOR_CHAR)
		{
			ft_dprintf(2, "{#ff3333}ERROR: illegal character %d after argument in %s\n{R}", *tmp, str);
			break ;
		}
		else if (!*tmp)
			break ;
		++tmp;
	}
	if (inst->i > g_op_tab[inst->op - 1].nb_param)
		ft_dprintf(2, "{#ff3333}ERROR: too many argument for instruction '%s', must have %d have %d\n{R}", g_op_tab[inst->op - 1].label, g_op_tab[inst->op - 1].nb_param, inst->i);
	else if (inst->i < g_op_tab[inst->op - 1].nb_param)
		ft_dprintf(2, "{#ff3333}ERROR: argument missing for instruction '%s', must have %d have %d\n{R}", g_op_tab[inst->op - 1].label, g_op_tab[inst->op - 1].nb_param, inst->i);
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
	if (e->actual->i < BIN_MAX_SIZE - 4 && e->actual->i + inst->index >= BIN_MAX_SIZE - 4) //fonction error
	{
		++e->actual->error;
		e->actual->i += inst->index;
	}
	if (!e->actual->error)
	{
		cp[e->actual->i++] = inst->op;
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
}

t_inst	*parse_inst(t_env *e, char *str, unsigned char *cp)
{	
	t_inst inst;
	char *tmp;

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
		ft_printf("{#ff3333}Error: unknown instruction '%s'\n", str);
	write_inst(e, &inst, cp);
	return (NULL);
}
