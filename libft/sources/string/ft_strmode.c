/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 21:07:35 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/07/20 05:10:03 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>

static char	mode_to_filetype(mode_t mode)
{
	if (mode == S_IFDIR)
		return ('d');
	else if (mode == S_IFCHR)
		return ('c');
	else if (mode == S_IFBLK)
		return ('b');
	else if (mode == S_IFREG)
		return ('-');
	else if (mode == S_IFLNK)
		return ('l');
	else if (mode == S_IFSOCK)
		return ('s');
#ifdef S_IFIFO
	else if (mode == S_IFIFO)
		return ('p');
#endif
#ifdef S_IFWHT
	else if (mode == S_IFWHT)
		return ('w');
#endif
	else
		return ('?');
}

static char exec_perm(mode_t mode, mode_t ex_mask, mode_t extend_mask, int oth)
{
	if (mode == ex_mask)
		return ('x');
	else if (mode == extend_mask)
		return (!oth ? 'S' : 'T');
	else if (mode == (ex_mask | extend_mask))
		return (!oth ? 's' : 't');
	else
		return ('-');
}

void		ft_strmode(mode_t mode, char *p)
{
	*p++ = mode_to_filetype(mode & S_IFMT);
	*p++ = mode & S_IRUSR ? 'r' : '-';
	*p++ = mode & S_IWUSR ? 'w' : '-';
	*p++ = exec_perm(mode & (S_IXUSR | S_ISUID), S_IXUSR, S_ISUID, 0);
	*p++ = mode & S_IRGRP ? 'r' : '-';
	*p++ = mode & S_IWGRP ? 'w' : '-';
	*p++ = exec_perm(mode & (S_IXGRP | S_ISGID), S_IXGRP, S_ISGID, 0);
	*p++ = mode & S_IROTH ? 'r' : '-';
	*p++ = mode & S_IWOTH ? 'w' : '-';
	*p++ = exec_perm(mode & (S_IXOTH | S_ISVTX), S_IXOTH, S_ISVTX, 1);
	*p++ = ' ';
	*p = '\0';
}
