/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 12:41:36 by matleroy          #+#    #+#             */
/*   Updated: 2019/05/06 21:44:32 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "asm.h"
#include <stdlib.h>

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
	if (e->file->error < MAX_ERROR && *tmp && *tmp != *SEPARATOR_CHAR && !ft_strchr(SPACES, *tmp))
		err += basic_error(e, tmp, "invalid parameter\n",
			param_strrspn(tmp, SPACES, *SEPARATOR_CHAR));
	tmp += ft_strcspn(tmp, SPACES",");
	tmp += ft_strspn(tmp, SPACES);
	if (*tmp && *tmp != *SEPARATOR_CHAR)
		err = 1;
	return (!err);
}

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

unsigned char	*lst_to_char(t_env *e, unsigned char *header, int *size)
{
	unsigned char	*str;
	t_buff			*buff;
	int				i;
	int				j;

	buff = e->file->begin_buff;
	i = HEADER_SIZE + buff->len;
	while ((buff = buff->next))
		i += buff->len;
	if (!(str = (unsigned char *)malloc(sizeof(unsigned char) * i)))
		alloc_error(e);
	*size = i;
	i = 0;
	while (i < HEADER_SIZE)
		str[i++] = *header++;
	buff = e->file->begin_buff;
	while (buff)
	{
		j = 0;
		while (j < buff->len)
			str[i++] = buff->s[j++];
		buff = buff->next;
	}
	return (str);
}
