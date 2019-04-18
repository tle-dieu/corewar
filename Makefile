PROGRAMMES = $(VM) $(ASM)
VM = corewar
ASM = asm

CFLAGS = -Wall -Werror -Wextra
CC = gcc $(CFLAGS)
LDFLAG = -L./$(LIBFT_DIR) -lft

RM = rm -rf

ASM_DIR = asm/
VM_DIR = corewar/

# --------------- Sources --------------- #

ASM_SOURCES_FILES = main.c \
					free.c \
					debug.c \
					args.c \
					error.c \
					utils_error.c \
					error_args.c \
					header.c

VM_SOURCES_FILES = main.c \
				   init.c \
				   print.c \
				   clean.c \
				   args.c \
				   debug.c \
				   check.c \
				   op_utils.c \
				   op_1_5.c \
				   op_6_10.c \
				   op_11_15.c \
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

ifneq (,$(filter $(fsanitize),y yes))
	CFLAGS += -g3
	CFLAGS += -fsanitize=address
endif

ifneq (,$(filter $(silent), y yes))
	SLEEP :=
	HIDE :=
	REDIRECT := > /dev/null
endif

# ifneq (,$(filter $(valgrind),y yes))
# 	RUN_OPTION += valgrind --leak-check=full --track-origins=yes --read-inline-info=yes --read-var-info=yes --num-callers=100 --show-possibly-lost=no
# 	CFLAGS += -g3
# endif

# ifneq (,$(filter $(time),y yes))
# 	RUN_OPTION = time -p
# endif
#

all: $(PROGRAMMES) Makefile


$(ASM): $(LIBFT) $(ASM_OBJECTS) Makefile
	$(SHOW)
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(CC) -o $(ASM) $(ASM_OBJECTS) $(LDFLAG)
	printf "$(GREEN)$(ASM) has been created$(RESET)\n" $(REDIRECT)

$(VM): $(LIBFT) $(VM_OBJECTS) Makefile
	$(SHOW)
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(CC) -o $(VM) $(VM_OBJECTS) $(LDFLAG)
	printf "$(GREEN)$(VM) has been created$(RESET)\n" $(REDIRECT)

$(ASM_OBJECTS_DIR)%.o: $(ASM_SOURCES_DIR)%.c $(ASM_INCLUDES) Makefile
	$(HIDE)
	mkdir -p $(dir $@)
	$(CC) -I $(INCLUDES_DIR) -I $(LIBFT_INCLUDES) -o $@ -c $< 
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(SLEEP)

$(VM_OBJECTS_DIR)%.o: $(VM_SOURCES_DIR)%.c $(VM_INCLUDES) Makefile
	$(HIDE)
	mkdir -p $(dir $@)
	$(CC) -I $(INCLUDES_DIR) -I $(LIBFT_INCLUDES) -o $@ -c $<
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(SLEEP)

$(LIBFT): force
	$(MAKE) silent=$(silent) fsanitize=$(fsanitize) flags=$(flags) -C $(LIBFT_DIR)

force:
	true

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

.SILENT: all $(PROGRAMMES) $(ASM_OBJECTS) $(LIBFT) $(VM_OBJECTS) force clean fclean run
