/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_param.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 15:07:09 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/09 17:43:36 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		inst_atoi(char *str)
{
	int					sign;
	unsigned long long	result;
	char				*tmp;
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

int		label_is_good(t_env *e, char *str)
{
	char	*tmp;
	int		err;

	err = 0;
	tmp = str;
	tmp += ft_strspn(tmp, LABEL_CHARS);
	if (e->file->error < 19 && *tmp != *SEPARATOR_CHAR
			&& *tmp && !ft_strchr(SPACES, *tmp))
	{
		basic_error(e, tmp, "illegal character for label\n", 0);
		err = 1;
	}
	tmp += ft_strcspn(tmp, END_PARAM);
	tmp += ft_strspn(tmp, SPACES);
	if (e->file->error < 19 && *tmp && *tmp != *SEPARATOR_CHAR)
	{
		basic_error(e, tmp, "unexpected expression after parameter\n",
				ft_strcspn(tmp, SEPARATOR_CHAR) - 1);
		err = 1;
	}
	err += (e->file->error >= MAX_ERROR);
	return (!err);
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
		err += basic_error(e, tmp, "illegal character for label\n", 0);
	tmp += ft_strcspn(tmp, SPACES",");
	tmp += ft_strspn(tmp, SPACES);
	if (e->file->error < MAX_ERROR && *tmp && *tmp != *SEPARATOR_CHAR)
		err += basic_error(e, tmp, "unexpected expression after parameter\n",
				ft_strcspn(tmp, END_PARAM) - 1);
	return (!err);
}

int		is_a_number(t_env *e, char *str)
{
	char	*tmp;
	int		err;

	err = 0;
	if (e->file->error < MAX_ERROR && *str == *SEPARATOR_CHAR)
		err += basic_error(e, str, "empty parameter before ','\n", 0);
	tmp = str + (*str == '-');
	while (ft_isdigit(*tmp))
		tmp++;
	if (e->file->error < MAX_ERROR && *tmp
			&& *tmp != *SEPARATOR_CHAR && !ft_strchr(SPACES, *tmp))
		err += basic_error(e, str, "invalid parameter\n",
				param_strrspn(tmp, SPACES, *SEPARATOR_CHAR));
	tmp += ft_strcspn(tmp, SPACES",");
	tmp += ft_strspn(tmp, SPACES);
	if (e->file->error < MAX_ERROR && *tmp && *tmp != *SEPARATOR_CHAR)
		err += basic_error(e, tmp, "unexpected expression after parameter\n",
				ft_strcspn(tmp, END_PARAM) - 1);
	return (!err);
}
