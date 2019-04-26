/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_words_reject.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 16:40:13 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/26 16:47:34 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_count_words_reject(char const *s, char c, char *reject)
{
	size_t words;

	words = 0;
	while (*s)
	{
		s += ft_strspn(s, reject);
		if (*s != c && *s)
			words++;
		while (*s != c && *s)
			s++;
		while (*s && *s == c)
			s++;
	}
	return (words);
}
