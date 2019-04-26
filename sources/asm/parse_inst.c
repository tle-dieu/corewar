/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/26 18:12:20 by tle-dieu         ###   ########.fr       */
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

int		is_reg(char *str)
{
	int reg;
	int i;
	char *tmp;

	tmp = str;
	reg = -1;
	i = 0;
	tmp += ft_strspn(str, SPACES);
	if (*tmp == 'r')
	{
		tmp++;
		while (tmp[i] && ft_isdigit(tmp[i]))
			++i;
		i += ft_strspn(tmp + i, SPACES);
		if (tmp[i])
		{
			ft_printf("{#ff3333}unexpected character %s i = %d\n", tmp + i, i);
			reg = -1;
		}
		else
			reg = ft_atoi(tmp);
		if (reg < 0 || reg > 16)
		{
			ft_printf("{#ff3333}bad register number %d{R}\n", reg);
			reg = -1;
		}
	}
	return (reg);
}

int is_indirect(char *str)
{
	int i;
	int nb;

	nb = 0;
	i = ft_strspn(str, SPACES);
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i++]))
		nb++;
	i += ft_strspn(str + i, SPACES);
	if (str[i])
		ft_printf("{#ff3333}unexpected character %s i = %d\n", str + i, i);
	return (nb);
}

int is_direct(char *str)
{
	int i;
	int nb;

	nb = 0;
	i = ft_strspn(str, SPACES);
	if (str[i] == DIRECT_CHAR)
	{
		i++;
		if (str[i] == '-')
			i++;
		while (str[i] && ft_isdigit(str[i++]))
			nb++;
		i += ft_strspn(str + i, SPACES);
		if (str[i])
			ft_printf("{#ff3333}unexpected character '%s' i = %d \n", str + i, i);
		if (!nb)
			ft_printf("{#ff3333}specified as direct but there is no digit\n");
	}
	return (nb);
}

int	is_a_label(char *str)
{
	int i;
	int dir;

	dir = 0;
	i = ft_strspn(str, SPACES);
	if (str[i] == DIRECT_CHAR)
	{
		dir = 1;
		++i;
	}
	if (str[i] && str[i] != LABEL_CHAR)
		return (0);
	i++;
	if (!ft_isalpha(str[i]))
		return (0);
	while (ft_isalpha(str[i]))
		i++;
	i += ft_strspn(SPACES, str + i);
	if (str[i])
		ft_printf("{#ff3333}bad character %s\n", str + i);
	return (1);
}

void get_ocp(t_inst *inst)
{
	int  i;
	int ocp;
	int max;

	i = 0;
	ocp = 0;
	max = 192;
	while (i < inst->nb_p)
	{
		max /= 3;
		ocp += max * inst->t[i];
		i++;
	}
	inst->ocp = ocp;
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

int		check_params(t_env *e, char **params, t_inst *inst)
{
	int i;
	int reg;
	int error;

	error = 0;
	reg = -1;
	i = 0;
	inst->index = e->actual->i + g_op_tab[inst->op - 1].ocp + 1;
	while (params[i])
	{
		inst->t[i] = 0;
		if (is_a_label(params[i]))
		{
			inst->s[i] = g_op_tab[inst->op - 1].dir_size && *params[i] == DIRECT_CHAR ? 2 : 4;
			get_label_call(e, inst, ft_strchr(params[i], LABEL_CHAR) + 1, i);
			ft_dprintf(2,"{R}[%d] label\n", i + 1);
		}
		else if (is_direct(params[i]))
		{
			inst->p[i] = ft_atoi(params[i] + 1);
			inst->s[i] = g_op_tab[inst->op - 1].dir_size ? 2 : 4;
			inst->t[i] = 2;
			ft_dprintf(2,"{R}[%d] direct = %d\n", i + 1, inst->p[i]);
		}
		else if (ft_strchr(params[i], 'r') && (reg = is_reg(params[i])) != -1)
		{
			inst->p[i] = reg;
			inst->s[i] = 1;
			inst->t[i] = 1;
			ft_dprintf(2,"{R}[%d] registre = %d\n", i + 1, inst->p[i]);
		}
		else if (is_indirect(params[i]))
		{
			inst->p[i] = ft_atoi(params[i]);
			inst->s[i] = IND_SIZE;
			inst->t[i] = 3;
			ft_dprintf(2,"{R}[%d] indirect = %d\n", i + 1, inst->p[i]);
		}
		else
			error = 1;
		if (!error) // changer error
		{
			ft_printf(STR_C"index: %02x :: %d -> %02x :: %d\n{R}", inst->index, inst->index, inst->index + inst->s[i], inst->index + inst->s[i]);
			inst->index += inst->s[i];
		}
		i++;
	}
	if (i != g_op_tab[inst->op - 1].nb_param)
		ft_printf("{#ff3333}error: wrong param number expected %d, get %d{R}\n", g_op_tab[inst->op - 1].nb_param, i);
	return (error || i != g_op_tab[inst->op - 1].nb_param);
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
	char **params;

	print_label(e);
	inst = (t_inst){.ocp = 0};
	ft_printf(NAME_C"\nparse_inst: {R}%s\n", str);
	if ((inst.op = get_curr_inst(str)) <= 16)
	{
		inst.nb_p = g_op_tab[inst.op - 1].nb_param;
		tmp = str + ft_strlen(g_op_tab[inst.op - 1].label);
		tmp += ft_strspn(tmp, SPACES);
		params = ft_strsplit(tmp, SEPARATOR_CHAR);
		check_params(e, params, &inst);
		if (g_op_tab[inst.op - 1].ocp)
			get_ocp(&inst);
	}
	write_inst(e, &inst, cp);
	print_inst(&inst, str);
	return (NULL);
}
