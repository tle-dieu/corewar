/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_champ.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:22:52 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/02 16:50:41 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BS 16
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//make -C ~/projects/libft && gcc write_champ.c -I libft/includes -L./libft/ -lft -o champ_wr

void	print_ascii(char *buff, int ret)
{
	int i;

	i = 0;
	while (ret + i++ < BS)
		ft_printf("  ");
	i = 0;
	while (i < BS)
	{
		ft_printf("%c", buff[i] >= 32 && buff[i] < 127 ? buff[i] : '.');
		i++;
	}
}

int		main(int ac, char **av)
{
	int		i;
	char	buff[BS];
	int		ret;
	int		j;
	int		rd_fd;
	int		wr_fd;

	if (ac == 2)
	{
		j = 0;
		rd_fd = open(av[1], O_RDONLY);
		wr_fd = open("test_wr.cor", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
		while ((ret = read(rd_fd, buff, BS)))
		{
			i = 0;
			ft_printf("%08x: ", j);
			while (i < BS)
			{
				if (i < ret)
				{
					ft_printf("%02x", 0xff & buff[i]);
					if (i + 1 < ret)
						ft_printf("%02x", 0xff & buff[i + 1]);
				}
				ft_printf(" ");
				i += 2;
			}
			j += i;
			print_ascii(buff, ret);
			ft_printf("\n");
			write(wr_fd, buff, ret);
		}
	}
}
