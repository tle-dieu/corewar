/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_bytes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 23:13:22 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/06/11 00:42:20 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned short	swap_bytes2(unsigned short bytes)
{
	return ((bytes >> 8) | ((bytes & 0xff) << 8));
}

unsigned		swap_bytes4(unsigned bytes)
{
	bytes = ((bytes & 0xffU << 24) >> 24) | ((bytes & 0xff << 16) >> 8)
		| ((bytes & 0xff << 8) << 8) | ((bytes & 0xff) << 24);
	return (bytes);
}

unsigned long	swap_bytes8(unsigned long bytes)
{
	bytes = ((bytes & 0xffUL << 56) >> 56) | ((bytes & 0xffUL << 48) >> 40)
		| ((bytes & 0xffUL << 40) >> 24) | ((bytes & 0xffUL << 32) >> 8)
		| ((bytes & 0xffU << 24) << 8) | ((bytes & 0xff << 16) << 24)
		| ((bytes & 0xff << 8) << 40) | ((bytes & 0xff) << 56);
	return (bytes);
}
