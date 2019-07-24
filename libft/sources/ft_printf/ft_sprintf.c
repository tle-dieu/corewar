/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 20:04:11 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/06/23 15:51:22 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_sprintf(char *str, const char *format, ...)
{
	va_list	arg;
	t_print	buff;

	if (!format)
		return (-1);
	va_start(arg, format);
	buff.i = 0;
	buff.total = 0;
	buff.error = 0;
	buff.fd = 1;
	buff.str = str;
	buff.print = 0;
	apply_format(arg, &buff, format);
	va_end(arg);
	buff.str[buff.i] = '\0';
	return (buff.error ? -1 : buff.i);
}
