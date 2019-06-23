# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/18 03:20:05 by tle-dieu          #+#    #+#              #
#    Updated: 2019/06/21 14:35:42 by tle-dieu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PROGRAMMES = $(VM) $(ASM)
VM = corewar
ASM = asm

CFLAGS = -Wall -Werror -Wextra
CC = clang
VISU = -lncurses
LDFLAG = -L./$(LIBFT_DIR) -lft

RM = rm -rf

ASM_DIR = asm/
VM_DIR = corewar/

# --------------- Sources --------------- #

ASM_SOURCES_FILES = main.c \
					args.c \
					options.c \
					assemble.c \
					line.c \
					header.c \
					parse_inst.c \
					check_inst.c \
					label.c \
					label_call.c \
					utils_error.c \
					error_args.c \
					error_cmd_format.c \
					error_cmd_global.c \
					error_global.c \
					error_inst.c \
					error_label.c \
					utils.c \
					free.c \
					disass.c \
					disass_utils.c \
					parse_disass.c \
					disass_lst_file.c \
					dump.c \
					op.c \
					param.c \
					check_param.c

VM_SOURCES_FILES = main.c \
				   init.c \
				   print.c \
				   clean.c \
				   args.c \
				   check.c \
				   op.c \
				   game_utils.c \
				   op_utils.c \
				   ocp_utils.c \
				   op_1_5.c \
				   op_6_10.c \
				   op_11_15.c \
				   visu.c \
				   visu_print.c \
				   play.c

SOURCES_DIR = sources/
VM_SOURCES_DIR = $(SOURCES_DIR)$(VM_DIR)
ASM_SOURCES_DIR = $(SOURCES_DIR)$(ASM_DIR)
VM_SOURCES = $(addprefix $(VM_SOURCES_DIR), $(VM_SOURCES_FILES))
ASM_SOURCES = $(addprefix $(ASM_SOURCES_DIR), $(ASM_SOURCES_FILES))

# --------------- Objects --------------- #

OBJECTS_DIR = objects/
VM_OBJECTS_DIR = $(OBJECTS_DIR)$(VM_DIR)
ASM_OBJECTS_DIR = $(OBJECTS_DIR)$(ASM_DIR)
VM_OBJECTS = $(addprefix $(VM_OBJECTS_DIR), $(VM_SOURCES_FILES:.c=.o))
ASM_OBJECTS = $(addprefix $(ASM_OBJECTS_DIR), $(ASM_SOURCES_FILES:.c=.o))

# --------------- Includes -------------- #

INCLUDES_DIR = includes/
ASM_INCLUDES = $(addprefix $(INCLUDES_DIR), asm.h op.h) $(LIBFT_INCLUDES)
VM_INCLUDES = $(addprefix $(INCLUDES_DIR), corewar.h op.h) $(LIBFT_INCLUDES)

# -------------- Libraries -------------- #

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
LIBFT_INCLUDES = $(LIBFT_DIR)includes/

# --------------- Style ---------------- #

GREEN = \033[38;2;12;231;58m
YELLOW = \033[38;2;251;196;15m
RED = \033[38;2;255;60;51m
BLUE = \033[38;2;0;188;218m
RMLINE = \033[2K
RESET = \033[0m
HIDE = tput civis
SHOW = tput cnorm
SLEEP = sleep 0.01

# --------------- Options --------------- #

ifneq (,$(filter $(flags),n no))
	CFLAGS =
endif

ifneq (,$(filter $(fsanitize), y yes))
	CFLAGS += -g3
ifeq ($(shell uname -s),Linux)
	CFLAGS += -fsanitize=address,undefined,integer,bounds,builtin
else
	CFLAGS += -fsanitize=address,undefined,integer,bounds
endif
endif

ifneq (,$(filter $(silent), y yes))
	SLEEP :=
	HIDE :=
	REDIRECT := > /dev/null
endif

all: $(PROGRAMMES) Makefile


$(ASM): $(LIBFT) $(ASM_OBJECTS) Makefile
	$(SHOW)
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(CC) $(CFLAGS) -o $(ASM) $(ASM_OBJECTS) $(LDFLAG)
	printf "$(GREEN)$(ASM) has been created$(RESET)\n" $(REDIRECT)

$(VM): $(LIBFT) $(VM_OBJECTS) Makefile
	$(SHOW)
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(CC) $(CFLAGS) $(VISU) -o $(VM) $(VM_OBJECTS) $(LDFLAG)
	printf "$(GREEN)$(VM) has been created$(RESET)\n" $(REDIRECT)

$(ASM_OBJECTS_DIR)%.o: $(ASM_SOURCES_DIR)%.c $(ASM_INCLUDES) Makefile
	$(HIDE)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -I $(LIBFT_INCLUDES) -o $@ -c $< 
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(SLEEP)

$(VM_OBJECTS_DIR)%.o: $(VM_SOURCES_DIR)%.c $(VM_INCLUDES) Makefile
	$(HIDE)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -I $(LIBFT_INCLUDES) -o $@ -c $< 
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(SLEEP)

$(LIBFT): force
	$(MAKE) silent=$(silent) fsanitize=$(fsanitize) flags=$(flags) -C $(LIBFT_DIR)

force:
	true

clean-test:
	find . -type f -name "*.cor" | grep -v invalid_cor | xargs $(RM)
	find . -type f -name "*_disass*.s" -o -name "*_disass*.cor" | xargs $(RM)
	git checkout @ resources/champs/invalid_cor

clean:
	$(MAKE) $@ -C $(LIBFT_DIR)
	$(RM) $(OBJECTS_DIR)
	printf "$(RED)The $(ASM) objects have been removed$(RESET)\n" $(REDIRECT)
	printf "$(RED)The $(VM) objects have been removed$(RESET)\n" $(REDIRECT)

fclean:
	$(MAKE) $@ -C $(LIBFT_DIR)
	$(RM) $(OBJECTS_DIR) $(PROGRAMMES)
	printf "$(RED)The $(ASM) objects have been removed$(RESET)\n" $(REDIRECT)
	printf "$(RED)The $(VM) objects have been removed$(RESET)\n" $(REDIRECT)
	printf "$(RED)$(ASM) has been removed$(RESET)\n" $(REDIRECT)
	printf "$(RED)$(VM) has been removed$(RESET)\n" $(REDIRECT)

re: fclean all

.PHONY: all clean fclean

.SILENT: all $(PROGRAMMES) $(ASM_OBJECTS) $(LIBFT) $(VM_OBJECTS) force clean clean-test fclean run
