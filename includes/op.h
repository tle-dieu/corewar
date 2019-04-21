/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 17:19:05 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/04/21 03:17:40 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 ** Toutes les tailles sont en octets.
 ** On part du principe qu'un int fait 32 bits. Est-ce vrai chez vous ?
 */

#ifndef OP_H
# define OP_H

# define IND_SIZE				2
# define REG_SIZE				4
# define DIR_SIZE				REG_SIZE

# define REG_CODE				1
# define DIR_CODE				2
# define IND_CODE				3

# define MAX_ARGS_NUMBER		4
# define MAX_PLAYERS			4
# define MEM_SIZE				(4*1024)
# define IDX_MOD				(MEM_SIZE / 8)
# define CHAMP_MAX_SIZE			(MEM_SIZE / 6)

# define COMMENT_CHAR			"#;"
# define LABEL_CHAR				':'
# define DIRECT_CHAR			'%'
# define SEPARATOR_CHAR			','

# define LABEL_CHARS			"abcdefghijklmnopqrstuvwxyz_0123456789"

# define NAME_CMD_STRING		".name"
# define COMMENT_CMD_STRING		".comment"

# define REG_NUMBER				16

# define CYCLE_TO_DIE			1536
# define CYCLE_DELTA			50
# define NBR_LIVE				21
# define MAX_CHECKS				10

typedef char			t_arg_type;

# define T_REG					1
# define T_DIR					2
# define T_IND					4
# define T_LAB					8

/* # define PROG_NAME_LENGTH		(128) */
# define PROG_NAME_LENGTH		(20)
# define COMMENT_LENGTH			(2048)
# define COREWAR_EXEC_MAGIC		0xea83f3
# define BIN_MAX_SIZE			PROG_NAME_LENGTH + COMMENT_LENGTH + CHAMP_MAX_SIZE + 16

# if PROG_NAME_LENGTH > COMMENT_LENGTH
# define BS_HEADER PROG_NAME_LENGTH
# else
# define BS_HEADER COMMENT_LENGTH
# endif

typedef struct			s_header
{
	unsigned int		magic;
	char				prog_name[PROG_NAME_LENGTH + 1];
	unsigned int		prog_size;
	char				comment[COMMENT_LENGTH + 1];
}						t_header;

typedef struct			s_op
{
	char				*label;
	int					len;
	int					nb_param;
	int					param[3];
	int					op_code;
	int					nb_cycle;
	void				(*ft_ptr)();
	int					jsaispas;
	int					dir_size;
}						t_op;

extern t_op				g_op_tab[17];

#endif
