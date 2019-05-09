/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaz <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/10/04 11:43:01 by zaz               #+#    #+#             */
/*   Updated: 2019/05/09 02:52:17 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include <string.h>

t_op	g_op_tab[17] =
{
	{"live", 4, 1, {T_DIR}, 1, 10, NULL, 0, 0},
	{"ld", 2, 2, {T_DIR | T_IND, T_REG}, 2, 5, NULL, 1, 0},
	{"st", 2, 2, {T_REG, T_IND | T_REG}, 3, 5, NULL, 1, 0},
	{"add", 3, 3, {T_REG, T_REG, T_REG}, 4, 10, NULL, 1, 0},
	{"sub", 3, 3, {T_REG, T_REG, T_REG}, 5, 10, NULL, 1, 0},
	{"and", 3, 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		NULL, 1, 0},
	{"or", 2, 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		NULL, 1, 0},
	{"xor", 3, 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		NULL, 1, 0},
	{"zjmp", 4, 1, {T_DIR}, 9, 20, NULL, 0, 1},
	{"ldi", 3, 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		NULL, 1, 1},
	{"sti", 3, 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		NULL, 1, 1},
	{"fork", 4, 1, {T_DIR}, 12, 800, NULL, 0, 1},
	{"lld", 3, 2, {T_DIR | T_IND, T_REG}, 13, 10, NULL, 1, 0},
	{"lldi", 4, 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		NULL, 1, 1},
	{"lfork", 5, 1, {T_DIR}, 15, 1000, NULL, 0, 1},
	{"aff", 3, 1, {T_REG}, 16, 2, NULL, 1, 0},
	{NULL, 0, 0, {0}, 0, 0, NULL, 0, 0}
};
