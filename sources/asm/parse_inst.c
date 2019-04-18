/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/18 14:01:35 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "op.h"

int get_curr_inst(t_op *op_tab, char *str)
{
	int i;
	int len;

	len = 0;
	i = 0;
	while (op_tab[i]->label && ft_strcmp(str, op_tab[i]->label))
		i++;
	len = ft_strlen(op_tab[i]->label);
	if (len && *(str + len) && (*str + len) == ' ')
	{
		ft_dprintf(2, "{#obd185}This is an instruction with op => %s !\n{reset}", op_tab[i]->label);
		return (i);
	}
	else if (len)
		ft_dprintf(2, "{#ff3333}This is not an instruction!\n{reset}");
	else
		ft_dprintf(2, "{#ff3333}Error: bad format (no space after instruction)!\n{reset}");
	return (42)
}

int		is_reg(char *str)
{
	int reg;
	int i;
	int j;
	
	j = 0;
	i = 0;
	reg = -1;
	while (str[i] && str[i] != 'r')
	{
		if (str[i] != ' ')
			reg = -1;
		i++;
	}
	if (str[i] == 'r')
	{
		i++;
		while (str[i + j] && ft_isdigit(str[i + j]))
			j++;
		reg = ft_atoi(str[i]);
		if (reg < 0 || reg > 16)
			reg = -1;
	}
	return (reg);
}

int is_indirect(char *str)
{
	int neg;
	int i;
	int nb;

	nb = 0;
	i = 0;
	neg = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '-')
		neg = 1;
	i += neg;
	while (str[i] && ft_isdigit(str[i++]))
		nb++;
	while (str[i])
	{	
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (nb);
}

int is_direct(char *str)
{
	int neg;
	int i;
	int nb;

	nb = 0;
	i = 0;
	neg = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == DIRECT_CHAR)
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (str[i] == '-')
			neg = 1;
		i += neg;
		while (str[i] && ft_isdigit(str[i++]))
			nb++;
		while (str[i])
		{	
			if (str[i] != ' ')
				return (0);
			i++;
		}
	}
	return (nb);
}

int	is_label(char *str)
{
	return (0);
}

int get_ocp(t_inst *inst, int nb_param)
{
	int  i;
	int ocp;
	int max;

	i = 0;
	ocp = 0;
	max = 192;
	while (i < nb_param)
	{
		max /= 3;
		ocp += max * inst->t[i];
		i++;
	}
	return (ocp);		
}

int		check_params(t_op *op_tab, char **params, t_inst *inst)
{
	int i;
	int reg;
	int error;

	error = 0;
	reg = -1;
	i = 0;
	while (params[i])
	{
		if (is_label(params[i]))
		{
			ft_dprintf(2,"Ce param est un label");
		}
		else if (is_direct(params[i]))
		{
			ft_dprintf(2,"Ce param est un direct");
			inst.p[i] = ft_atoi(params[i]);
			inst.s[i] = op_tab[inst->op - 1]->dir_size ? 2 : 4;
			inst.t[i] = 2;
		}
		else if (ft_strchr(params[i], 'r') && (reg = check_reg(params[i])) != -1)
		{
			inst.p[i] = reg;
			inst.s[i] = REG_SIZE;
			inst.t[i] = 1;
		}
		else if (is_indirect(params[i]))
		{
			inst.p[i] = ft_atoi(params[i]);
			inst.s[i] = IND_SIZE;
			inst.t[i] = 3;
		}
		else 
			error = 1;
		i++;
	}
	return (error || i > op_tab[inst->op - 1]->nb_param);
}

t_inst	*parse_inst(t_op *op_tab, char *str)
{	
	t_inst inst;
	char *tmp;
	char **params;
	
	inst = NULL;
	if (( inst->op = opget_curr_inst(str)) < 16 && (inst = (t_inst*)malloc(sizeof(t_inst))))
	{
		tmp = str + ft_strlen(op_tab[inst.op].label);
		while (*tmp && *tmp == ' ')
			tmp++;
		params = ft_strsplit(tmp, SEPARATOR_CHAR);
		check_params(op_tab, params, &inst);
	}
	return (inst)	
}
