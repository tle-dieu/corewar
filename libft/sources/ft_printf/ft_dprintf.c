/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 13:29:49 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/06/23 15:24:15 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ft_dprintf(int fd, char const *format, ...)
{
	va_list	arg;
	t_print	buff;

	if (!format)
		return (-1);
	va_start(arg, format);
	buff.i = 0;
	buff.total = 0;
	buff.error = 0;
	buff.fd = fd;
	buff.str = buff.sbuff;
	buff.print = 1;
	apply_format(arg, &buff, format);
	empty_buff(&buff);
	va_end(arg);
	return (buff.error ? -1 : buff.total);
}
