PROGRAMMES = $(VM) $(ASM)
VM = corewar
ASM = asm

CFLAG = -Wall -Werror -Wextra
CC = gcc $(CFLAG)
LDFLAG = -L./$(LIBFT_FOLDER) -lft

RM = rm -rf

ASM_FOLDER = asm/
VM_FOLDER = corewar/

# --------------- Sources --------------- #

ASM_SOURCES_FILES = main.c \
					debug.c

VM_SOURCES_FILES = main.c \
				   options.c \
				   debug.c

SOURCES_FOLDER = sources/
VM_SOURCES_FOLDER = $(SOURCES_FOLDER)$(VM_FOLDER)
ASM_SOURCES_FOLDER = $(SOURCES_FOLDER)$(ASM_FOLDER)
VM_SOURCES = $(addprefix $(VM_SOURCES_FOLDER), $(VM_SOURCES_FILES))
ASM_SOURCES = $(addprefix $(ASM_SOURCES_FOLDER), $(ASM_SOURCES_FILES))

# --------------- Objects --------------- #

OBJECTS_FOLDER = objects/
VM_OBJECTS_FOLDER = $(OBJECTS_FOLDER)$(VM_FOLDER)
ASM_OBJECTS_FOLDER = $(OBJECTS_FOLDER)$(ASM_FOLDER)
VM_OBJECTS = $(addprefix $(VM_OBJECTS_FOLDER), $(VM_SOURCES_FILES:.c=.o))
ASM_OBJECTS = $(addprefix $(ASM_OBJECTS_FOLDER), $(ASM_SOURCES_FILES:.c=.o))

# --------------- Includes -------------- #

INCLUDES_FOLDER = includes/
ASM_INCLUDES = $(addprefix $(INCLUDES_FOLDER), asm.h op.h) $(LIBFT_INCLUDES)
VM_INCLUDES = $(addprefix $(INCLUDES_FOLDER), corewar.h op.h) $(LIBFT_INCLUDES)

# -------------- Libraries -------------- #

LIBFT_FOLDER = libft/
LIBFT = $(LIBFT_FOLDER)libft.a
LIBFT_INCLUDES = $(LIBFT_FOLDER)includes/

# --------------- Colors ---------------- #

GREEN = \033[38;2;12;231;58m
YELLOW = \033[38;2;251;196;15m
RED = \033[38;2;255;60;51m
BLUE = \033[38;2;0;188;218m
RMLINE = \033[2K
RESET = \033[0m

# --------------- Options --------------- #

ifneq (,$(filter $(flags),n no))
	CFLAG =
endif

ifneq (,$(filter $(fsanitize),y yes))
	CFLAG += -g3
	CFLAG += -fsanitize=address
endif

# ifneq (,$(filter $(valgrind),y yes))
# 	RUN_OPTION += valgrind --leak-check=full --track-origins=yes --read-inline-info=yes --read-var-info=yes --num-callers=100 --show-possibly-lost=no
# 	CFLAG += -g3
# endif

# ifneq (,$(filter $(time),y yes))
# 	RUN_OPTION = time -p
# endif
#

all: $(PROGRAMMES) Makefile


$(ASM): $(LIBFT) $(ASM_OBJECTS) Makefile
	tput cnorm
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n"
	$(CC) -o $(ASM) $(ASM_OBJECTS) $(LDFLAG)
	printf "$(GREEN)$(ASM) has been created$(RESET)\n"

$(VM): $(LIBFT) $(VM_OBJECTS) Makefile
	tput cnorm
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n"
	$(CC) -o $(VM) $(VM_OBJECTS) $(LDFLAG)
	printf "$(GREEN)$(VM) has been created$(RESET)\n"

$(ASM_OBJECTS_FOLDER)%.o: $(ASM_SOURCES_FOLDER)%.c $(ASM_INCLUDES) Makefile
	tput civis
	mkdir -p $(dir $@)
	$(CC) -I $(INCLUDES_FOLDER) -I $(LIBFT_INCLUDES) -o $@ -c $<
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r"
	sleep 0.02

$(VM_OBJECTS_FOLDER)%.o: $(VM_SOURCES_FOLDER)%.c $(VM_INCLUDES) Makefile
	tput civis
	mkdir -p $(dir $@)
	$(CC) -I $(INCLUDES_FOLDER) -I $(LIBFT_INCLUDES) -o $@ -c $<
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r"
	sleep 0.02

$(LIBFT): force
	$(MAKE) -C $(LIBFT_FOLDER)

force:
	true

clean:
	$(MAKE) $@ -C $(LIBFT_FOLDER)
	$(RM) $(OBJECTS_FOLDER)
	printf "$(RED)The $(ASM) objects have been removed$(RESET)\n"
	printf "$(RED)The $(VM) objects have been removed$(RESET)\n"

fclean:
	$(MAKE) $@ -C $(LIBFT_FOLDER)
	$(RM) $(OBJECTS_FOLDER) $(PROGRAMMES)
	printf "$(RED)The $(ASM) objects have been removed$(RESET)\n"
	printf "$(RED)The $(VM) objects have been removed$(RESET)\n"
	printf "$(RED)$(ASM) has been removed$(RESET)\n"
	printf "$(RED)$(VM) has been removed$(RESET)\n"

re: fclean all

.PHONY: all clean fclean

.SILENT: all $(PROGRAMMES) $(ASM_OBJECTS) $(VM_OBJECTS) $(LIBFT) force clean fclean
