PROGRAMMES = $(VM) $(ASM)
VM = corewar
ASM = asm

CFLAG = -Wall -Werror -Wextra
CC = gcc $(CFLAG)
LDFLAG = -L./$(LIBDIR) -lft

RM = rm -rf

ASM_FOLDER = asm/
VM_FOLDER = VM/
SOURCES_FOLDER = sources/
INCLUDES_FOLDER = includes/
OBJECTS_FOLDER = objects/
DEPS_FOLDER = .d/

vpath %.c $(SOURCES_FOLDER)

ASM_SOURCES = main.c \
			  debug.c

VM_SOURCES = main.c \
			 options.c

DEPS = $(patsubst $(OBJECTS_FOLDER)%, $(DEPS_FOLDER)%, $(patsubst %.o, %.d, $@))

VM_SOURCES := $(addprefix $(VM_FOLDER), $(VM_SOURCES))
ASM_SOURCES := $(addprefix $(ASM_FOLDER), $(ASM_SOURCES))

ASM_OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(ASM_SOURCES:.c=.o))
VM_OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(VM_SOURCES:.c=.o))

LIBDIR = libft/
LIBFT = $(LIBDIR)libft.a
LIBINC = $(LIBDIR)includes/

GREEN = \033[38;2;12;231;58m
YELLOW = \033[38;2;251;196;15m
RED = \033[38;2;255;60;51m
BLUE = \033[38;2;0;188;218m
RMLINE = \033[2K
RESET = \033[0m

ifneq (,$(filter $(flags),n no))
	CFLAG :=
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
# 	RUN_OPTION := time -p
# endif

$(shell mkdir -p $(DEPS_FOLDER)$(VM_FOLDER) $(DEPS_FOLDER)$(ASM_FOLDER) 2> /dev/null)

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

objects/%.o: %.c includes/asm.h
	tput civis
	mkdir -p $(dir $@)
	$(CC) -I $(INCLUDES_FOLDER) -I $(LIBINC) -o $@ -c $<
	$(CC) -MM -I $(INCLUDES_FOLDER) -I $(LIBINC) $< > $(DEPS)
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r"
	sleep 0.02

$(LIBFT): force
	$(MAKE) -C $(LIBDIR)

force:
	true

clean:
	$(MAKE) $@ -C $(LIBDIR)
	$(RM) $(OBJECTS_FOLDER)
	printf "$(RED)The $(ASM) objects have been removed$(RESET)\n"
	printf "$(RED)The $(VM) objects have been removed$(RESET)\n"

fclean:
	$(MAKE) $@ -C $(LIBDIR)
	$(RM) $(OBJECTS_FOLDER) $(PROGRAMMES)
	printf "$(RED)The $(ASM) objects have been removed$(RESET)\n"
	printf "$(RED)The $(VM) objects have been removed$(RESET)\n"
	printf "$(RED)$(ASM) has been removed$(RESET)\n"
	printf "$(RED)$(VM) has been removed$(RESET)\n"

re: fclean all

.PHONY: all clean fclean

.SILENT: $(PROGRAMMES) $(ASM_OBJECTS) $(VM_OBJECTS) $(LIBFT) force clean fclean
