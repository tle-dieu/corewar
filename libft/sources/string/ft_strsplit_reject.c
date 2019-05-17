/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit_reject.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 16:39:41 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/05/18 01:39:08 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	**ft_strsplit_reject(char const *s, char c, char *reject)
{
	char	**split;
	size_t	words;
	size_t	i;
	size_t	j;
	size_t	k;

	if (!s)
		return (NULL);
	words = ft_count_words_reject(s, c, reject);
	if (!(split = (char **)malloc(sizeof(char *) * (words + 1))))
		return (NULL);
	j = 0;
	while (j < words && !(i = 0))
	{
		k = 0;
		while (*s && (*s == c || ft_strchr(reject, *s)))
			s++;
		while (s[i + k] != c && s[i + k]
			&& (!ft_strchr(reject, s[i + k]) || !++k))
			i++;
		split[j++] = ft_strsub(s, 0, i);
		s += i + k;
	}
	split[j] = NULL;
	return (split);
}
