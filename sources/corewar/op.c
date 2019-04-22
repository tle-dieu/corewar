/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaz <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/10/04 11:43:01 by zaz               #+#    #+#             */
/*   Updated: 2019/04/22 15:56:13 by acompagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_op	g_op_tab[17] =
{
	{"live", 1, {T_DIR}, 1, 10, &live, 0, 0},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, &ld, 1, 0},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, &st, 1, 0},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, &add, 1, 0},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, &sub, 1, 0},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		&and, 1, 0},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		&or, 1, 0},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		&xor, 1, 0},
	{"zjmp", 1, {T_DIR}, 9, 20, &zjmp, 0, 1},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		&ldi, 1, 1},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		&sti, 1, 1},
	{"fork", 1, {T_DIR}, 12, 800, &op_fork, 0, 1},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, &lld, 1, 0},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		&lldi, 1, 1},
	{"lfork", 1, {T_DIR}, 15, 1000, &lfork, 0, 1},
	{"aff", 1, {T_REG}, 16, 2, &aff, 1, 0},
	{0, 0, {0}, 0, 0, 0, 0, 0}
};
