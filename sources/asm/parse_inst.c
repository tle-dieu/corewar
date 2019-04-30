/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 16:43:51 by matleroy          #+#    #+#             */
/*   Updated: 2019/04/30 23:49:28 by matleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"
#include <stdlib.h>

size_t	param_strrspn(const char *s, const char *accept, char stop)
{
	const char	*tmp;
	size_t		i;

	i = ft_strclen(s, stop) - 1;
	while (i > 0)
	{
		tmp = accept;
		while (*tmp != s[i])
			if (!*tmp++)
				return (i);
		--i;
	}
	return (i);
}

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
		new->next = e->file->label;
		e->file->label = new;
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
	e->file->last->free = 0;
	call->line = e->file->last;
	call->index_inst = e->file->i;
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

	label = e->file->label;
	len = ft_strcspn(s, SPACES",");
	e->i = len;
	while (label)
	{
		if (!ft_strncmp(s, label->name, len) && !label->name[len])
		{
			if (label->index != -1)
			{
				inst->p[i] = label->index - e->file->i;
				return ;
			}
			break ;
		}
		label = label->next;
	}
	if (!label || label->index == -1)
		create_call(e, inst, s, label, i); // separer en deux pour trop args
}

int		is_a_number(t_env *e, char *str)
{
	char *tmp;
	int err;

	err = 0;
	tmp = str + (*str == '-');
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != *SEPARATOR_CHAR && *tmp && !ft_strchr(SPACES, *tmp))
	{
		basic_error(e, tmp,"Invalid parameter\n", param_strrspn(tmp, SPACES, *SEPARATOR_CHAR));
		err = 1;
	}
	tmp += ft_strcspn(tmp, SPACES SEPARATOR_CHAR);
	tmp += ft_strspn(tmp, SPACES);
	if (*tmp && *tmp != *SEPARATOR_CHAR)
	{
		err = 1;
		basic_error(e, tmp,"unexpected expression after parameter\n", param_strrspn(tmp, SPACES, *SEPARATOR_CHAR));
	}
	return (!err);
}

int	label_is_good(t_env *e, char *str)
{
	char *tmp;
	int err;

	err = 0;
	tmp = str;
	tmp += ft_strspn(tmp, LABEL_CHARS);
	if (*tmp != *SEPARATOR_CHAR && *tmp && !ft_strchr(SPACES, *tmp))
	{
		basic_error(e, tmp,"illegal character for label\n", 0);
		err = 1;
	}
	tmp += ft_strcspn(tmp, SPACES SEPARATOR_CHAR);
	tmp += ft_strspn(tmp, SPACES);
	if (*tmp && *tmp != *SEPARATOR_CHAR)
	{
		basic_error(e, tmp,"unexpected expression after parameter\n", ft_strcspn(tmp, SEPARATOR_CHAR) - 1);
		err = 1;
	}
	return (!err);
}

int inst_atoi(char *str)
{
	int					sign;
	unsigned long long	result;
	char *tmp;
	int					i;

	i = 0;
	tmp = str;
	result = 0;
	if (*tmp && *tmp != *SEPARATOR_CHAR)
		sign = (*tmp == '-' ? -1 : 1);
	if (*tmp == '+' || *tmp == '-')
		tmp++;
	while (tmp[i] >= '0' && tmp[i] <= '9')
		result = result * 10 + tmp[i++] - 48;
	if (i > 19 || result > 9223372036854775807)
		return (sign < 0 ? 0 : -1);
	return ((int)result * sign);
}

int is_direct(t_env *e, char *str, t_inst *inst)
{
	char *tmp;

	tmp = str;
	if (*tmp == DIRECT_CHAR)
	{
		++tmp;
		inst->s[inst->i] = g_op_tab[inst->op - 1].dir_size ? 2 : 4;
		inst->t[inst->i] = 2;
		if (*tmp == LABEL_CHAR)
		{
			if (label_is_good(e, tmp + 1))
				get_label_call(e, inst, tmp + 1, inst->i);
		}
		else
		{
			if (is_a_number(e, tmp))
				inst->p[inst->i] = inst_atoi(tmp);
		}
		return (1);
	}
	return (0);
}

int is_indirect(t_env *e, char *str, t_inst *inst)
{
	char *tmp;

	tmp = str;
	inst->s[inst->i] = IND_SIZE;
	inst->t[inst->i] = 3;
	if (*tmp == LABEL_CHAR)
	{
		if (label_is_good(e, tmp + 1))
			get_label_call(e, inst, tmp + 1, inst->i);
	}
	else
	{
		if (is_a_number(e, tmp))
			inst->p[inst->i] = inst_atoi(tmp);
		else
			return (0);
	}
	return (1);
}

int		is_valid_register(t_env *e, char *str)
{
	char	*tmp;
	int		err;

	err = 0;
	tmp = str;
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != *SEPARATOR_CHAR && *tmp && !ft_strchr(SPACES, *tmp))
	{
		basic_error(e, tmp,"illegal character for label\n", 0);
		err = 1;
	}
	tmp += ft_strcspn(tmp, SPACES SEPARATOR_CHAR);
	tmp += ft_strspn(tmp, SPACES);
	if (*tmp && *tmp != *SEPARATOR_CHAR)
	{
		basic_error(e, tmp,"unexpected expression after parameter\n", ft_strcspn(tmp, SEPARATOR_CHAR) - 1);
		err = 1;
	}
	return (!err);
}

void	error_register_nb(t_env *e, char *str, int nb)
{
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, e->file->last->y , str - e->file->last->s);
	ft_dprintf(2, "register index must be between 1 and 16 have %d\n", nb);
	err_pointer(e->tty2, e->file->last->s, str, 0);
	err_wave(e->tty2, str, ft_strspn(str, "0123456789") - 1);	
	ft_dprintf(2, "\n");
}

void	error_unknow_inst(t_env *e, char *str)
{
		e->file->error++;
		ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, e->file->last->y , str - e->file->last->s);
		ft_dprintf(2, "unknow instruction %s\n", str);
		err_pointer(e->tty2, e->file->last->s, str, 0);
		err_wave(e->tty2, str, param_strrspn(str, SPACES, 0));	
		ft_dprintf(2, "\n");
}
int		is_reg(t_env *e, char *str, t_inst *inst)
{
	char *tmp;

	tmp = str;
	if (*tmp == 'r')
	{
		tmp++;
		inst->s[inst->i] = 1;
		inst->t[inst->i] = 1;
		if (is_valid_register(e, tmp))
			inst->p[inst->i] = inst_atoi(tmp);
		if (inst->p[inst->i] < 0 || inst->p[inst->i] > REG_NUMBER)
			error_register_nb(e, tmp, inst->p[inst->i]);
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

void	error_nb_param(t_env *e, char *str, int have, int should_have)
{
	e->file->error++;
	ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, e->file->last->y , str - e->file->last->s);
	if (have > should_have)
		ft_dprintf(2, "too many parameter, have %d parameter must have %d\n", have, should_have);
	else
		ft_dprintf(2, "missing parameter, have %d parameter must have %d\n", have, should_have);
	err_pointer(e->tty2, e->file->last->s, str, 0);
	err_wave(e->tty2, str, param_strrspn(str, SPACES, 0));	
	ft_dprintf(2, "\n");
}

void error_param_type(t_env *e, t_inst *inst, char *str)
{
	int type;
	int op_type;
	const char *types[4] = {REGISTER, DIRECT, "", INDIRECT};
	
	type = inst->t[inst->i];
	op_type = g_op_tab[inst->op - 1].param[inst->i];
	if (type == 3)
		type = 4;
	if (g_op_tab[inst->op - 1].nb_param > inst->i && !(type & op_type))
	{
		e->file->error++;
		ft_dprintf(2, line_error(ERR_LINE, e->tty2), e->file->name, e->file->last->y , str - e->file->last->s);
		ft_dprintf(2, "parameter[%d] type is %s, expected type (", inst->i, types[inst->t[inst->i]]);// tableau de macro
		if (op_type & T_IND)
			ft_dprintf(2, INDIRECT) && ((op_type & T_DIR) || (op_type & T_REG)) && ft_dprintf(2, " | ");
		if (op_type & T_DIR)
			ft_dprintf(2, DIRECT) && (op_type & T_DIR) && ft_dprintf(2, " | ");
		if (op_type & T_DIR)
			ft_dprintf(2, REGISTER);
		ft_dprintf(2, ") for instruction '%s'", g_op_tab[inst->op - 1].label);
		ft_dprintf(2, "\n");
		err_pointer(e->tty2, e->file->last->s, str, 0);
		err_wave(e->tty2, str, param_strrspn(str, SPACES, 0));	
		ft_dprintf(2, "\n");
	}
}

int check_params(t_env *e, char *str, t_inst *inst)
{
	char *tmp;
	char *begin;

	tmp = str;
	inst->index = e->file->i + g_op_tab[inst->op - 1].ocp + 1;
	while (*tmp)
	{
		tmp += ft_strspn(tmp, SPACES);
		if (g_op_tab[inst->op - 1].nb_param >= inst->i)
			begin = tmp;
		if (g_op_tab[inst->op - 1].nb_param > inst->i)
			is_direct(e, tmp, inst) || is_reg(e, tmp, inst) || is_indirect(e, tmp, inst);
		tmp += ft_strspn(tmp, SPACES);
		tmp += ft_strcspn(tmp, SEPARATOR_CHAR);
		if (g_op_tab[inst->op - 1].nb_param > inst->i)
		{
			error_param_type(e, inst, begin);
			inst->index += inst->s[inst->i];
		}
		inst->i++;
		if (!*tmp)
			break ;
		if (*tmp++ == ',' && !tmp[ft_strspn(tmp, SPACES)])
			basic_error(e, tmp - 1,"expected parameter after ','\n", 0);
	}
	if (inst->i != g_op_tab[inst->op - 1].nb_param)
		error_nb_param(e, begin, inst->i, g_op_tab[inst->op - 1].nb_param);
	return (1);
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
