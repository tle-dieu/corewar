/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 17:27:49 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/18 13:18:25 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <string.h>

# define BS_GNL 100
# define MAX_SIZE_STATIC 10000

typedef struct		s_gnl
{
	char			*str;
	int				fd;
	size_t			len;
	struct s_gnl	*next;
}					t_gnl;

#endif
