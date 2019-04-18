/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst->c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/18 18:10:43 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"

int get_curr_inst(char *str)
{
	int i;
	int len;

	
	ft_printf(FT_C"get_curr_inst\n{R}");
	len = 0;
	i = 0;
	while (g_op_tab[i].label  
	&& (ft_strncmp(str, g_op_tab[i].label, g_op_tab[i].len) 
	|| (str[g_op_tab[i].len] != '\t' && str[g_op_tab[i].len] != ' ')))
		i++;
	if (!g_op_tab[i].label)
		return (42);	
	ft_dprintf(2, "{yellow}This is instruction \"%s\" with op %d!\n{reset}", g_op_tab[i].label, i + 1);
	return (i + 1);
}

int		is_reg(char *str)
{
	int reg;
	int i;
	int j;
	
	ft_printf("is_reg\n");
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
		reg = ft_atoi(str + i);
		if (reg < 0 || reg > 16)
			reg = -1;
	}
	ft_printf("found reg of %d\n", reg);
	return (reg);
}

int is_indirect(char *str)
{
	int neg;
	int i;
	int nb;

	ft_printf("is_indirect\n");
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
		if (str[i] != ' ' || str[i] != '\t')
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

	ft_printf("is_direct >>>>>> %s\n", str);
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
		ft_printf("dir of %d\n", nb);
		ft_printf("=======%s\n", str + i);
	}
	ft_printf("found dir of %d\n", nb);
	return (nb);
}

int	is_label(char *str)
{
	ft_printf(FT_C"is_label\n");
	(void)str;
	return (0);
}

int get_ocp(t_inst *inst, int nb_param)
{
	int  i;
	int ocp;
	int max;

	ft_printf(FT_C"get_ocp\n");
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

int		check_params(char **params, t_inst *inst)
{
	int i;
	int reg;
	int error;

	ft_printf(FT_C"check_params\n");
	error = 0;
	reg = -1;
	i = 0;
	while (params[i] && i < 3)
	{
		inst->t[i] = 0;
		if (is_label(params[i]))
		{
			ft_dprintf(2,"Ce param est un label");
		}
		else if (is_direct(params[i]))
		{
			ft_dprintf(2,"Ce param est un direct");
			inst->p[i] = ft_atoi(params[i]);
			inst->s[i] = g_op_tab[inst->op - 1].dir_size ? 2 : 4;
			inst->t[i] = 2;
		}
		else if (ft_strchr(params[i], 'r') && (reg = is_reg(params[i])) != -1)
		{
			inst->p[i] = reg;
			inst->s[i] = REG_SIZE;
			inst->t[i] = 1;
		}
		else if (is_indirect(params[i]))
		{
			inst->p[i] = ft_atoi(params[i]);
			inst->s[i] = IND_SIZE;
			inst->t[i] = 3;
		}
		else 
			error = 1;
		i++;
	}
	return (error || i > g_op_tab[inst->op - 1].nb_param);
}

void print_inst(t_inst *inst, char *str)
{
	int i;

	i = 0;
	ft_printf(CHAMP_C"\n----------INSTRUCTION-----------\n{R}");
	ft_printf("str = %s\n", str);
	ft_printf("\top = %d\n", inst->op);
	ft_printf("\tocp = %d\n", inst->ocp);
	ft_printf("\tnb_p = %d\n", inst->nb_p);
	ft_printf("\tparams\n");
	while (i < inst->nb_p)
	{
		ft_printf("\t\t p[%d] = %d\n", i, inst->p[i]);
		ft_printf("\t\t s[%d] = %d\n", i, inst->s[i]);
		ft_printf("\t\t t[%d] = %d\n", i, inst->t[i]);
		i++;
	}
}

t_inst	*parse_inst(char *str)
{	
	t_inst inst;
	char *tmp;
	char **params;

	inst.nb_p = 0;
	ft_printf(FT_C"{reset}parse_inst\n");
	if ((inst.op = get_curr_inst(str)) <= 16)
	{
		inst.nb_p = g_op_tab[inst.op - 1].nb_param;
		tmp = str + ft_strlen(g_op_tab[inst.op].label);
		while (*tmp && *tmp == ' ')
			tmp++;
		params = ft_strsplit(tmp, SEPARATOR_CHAR);
		check_params(params, &inst);
	}
	print_inst(&inst, str);
	return (NULL);
}
