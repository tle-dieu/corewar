NAME = libft.a
NAME_SHARED = libft.so
CC = gcc
CFLAGS = -Wall -Werror -Wextra

AR = ar rc
RLIB = ranlib
RM = rm -rf

SOURCES_FOLDER = sources/
OBJECTS_FOLDER = objects/
INCLUDES_FOLDER = includes/

vpath %.c $(SOURCES_FOLDER)

STRING = string/
NUMERIC = numeric/
FT_PRINTF = ft_printf/
UTILS = utils/
LIST = list/
MEMORY = memory/
PRINT = printf

SOURCES = $(FT_PRINTF)buff.c \
		  $(FT_PRINTF)colors.c \
		  $(FT_PRINTF)conv_cs.c \
		  $(FT_PRINTF)conv_di.c \
		  $(FT_PRINTF)conv_f.c \
		  $(FT_PRINTF)conv_lcls.c \
		  $(FT_PRINTF)conv_oux.c \
		  $(FT_PRINTF)conv_p.c \
		  $(FT_PRINTF)flags.c \
		  $(FT_PRINTF)ft_dprintf.c \
		  $(FT_PRINTF)ftoa_binary.c \
		  $(FT_PRINTF)ftoa_bint.c \
		  $(FT_PRINTF)ftoa_calc.c \
		  $(FT_PRINTF)ftoa_list.c \
		  $(FT_PRINTF)ft_printf.c \
		  $(FT_PRINTF)utils.c \
		  $(STRING)ft_count_occ.c \
		  $(STRING)ft_ncount_occ.c \
		  $(STRING)ft_count_words.c \
		  $(STRING)ft_strcat.c \
		  $(STRING)ft_strcdup.c \
		  $(STRING)ft_strchr.c \
		  $(STRING)ft_strclen.c \
		  $(STRING)ft_strclr.c \
		  $(STRING)ft_strcmp.c \
		  $(STRING)ft_strcmpi.c \
		  $(STRING)ft_strcpy.c \
		  $(STRING)ft_strcspn.c \
		  $(STRING)ft_strdel.c \
		  $(STRING)ft_strdup.c \
		  $(STRING)ft_strequ.c \
		  $(STRING)ft_striter.c \
		  $(STRING)ft_striteri.c \
		  $(STRING)ft_strjoin.c \
		  $(STRING)ft_strjoin_sep.c \
		  $(STRING)ft_strlcat.c \
		  $(STRING)ft_strlen.c \
		  $(STRING)ft_strlwr.c \
		  $(STRING)ft_strmap.c \
		  $(STRING)ft_strmapi.c \
		  $(STRING)ft_strncat.c \
		  $(STRING)ft_strncmp.c \
		  $(STRING)ft_strncpy.c \
		  $(STRING)ft_strndup.c \
		  $(STRING)ft_strnequ.c \
		  $(STRING)ft_strnew.c \
		  $(STRING)ft_strnstr.c \
		  $(STRING)ft_strpbrk.c \
		  $(STRING)ft_strrchr.c \
		  $(STRING)ft_strrev.c \
		  $(STRING)ft_strsplit.c \
		  $(STRING)ft_strspn.c \
		  $(STRING)ft_strstr.c \
		  $(STRING)ft_strsub.c \
		  $(STRING)ft_strtrim.c \
		  $(STRING)ft_strupr.c \
		  $(STRING)ft_tolower.c \
		  $(STRING)ft_toupper.c \
		  $(NUMERIC)ft_atoi.c \
		  $(NUMERIC)ft_atoll.c \
		  $(NUMERIC)ft_isalnum.c \
		  $(NUMERIC)ft_isalpha.c \
		  $(NUMERIC)ft_isascii.c \
		  $(NUMERIC)ft_isdigit.c \
		  $(NUMERIC)ft_isprint.c \
		  $(NUMERIC)ft_iterative_power.c \
		  $(NUMERIC)ft_itoa.c \
		  $(NUMERIC)ft_sort_integer_table.c \
		  $(NUMERIC)ft_sqrt.c \
		  $(MEMORY)ft_bzero.c \
		  $(MEMORY)ft_memalloc.c \
		  $(MEMORY)ft_memccpy.c \
		  $(MEMORY)ft_memchr.c \
		  $(MEMORY)ft_memcmp.c \
		  $(MEMORY)ft_memcpy.c \
		  $(MEMORY)ft_memdel.c \
		  $(MEMORY)ft_memdup.c \
		  $(MEMORY)ft_memjoin.c \
		  $(MEMORY)ft_memmove.c \
		  $(MEMORY)ft_memset.c \
		  $(LIST)ft_lstadd.c \
		  $(LIST)ft_lstdel.c \
		  $(LIST)ft_lstdelone.c \
		  $(LIST)ft_lstfind.c \
		  $(LIST)ft_lstiter.c \
		  $(LIST)ft_lstlen.c \
		  $(LIST)ft_lstmap.c \
		  $(LIST)ft_lstnew.c \
		  $(UTILS)ft_print_words_tables.c \
		  $(UTILS)ft_putchar.c \
		  $(UTILS)ft_putchar_fd.c \
		  $(UTILS)ft_putendl.c \
		  $(UTILS)ft_putendl_fd.c \
		  $(UTILS)ft_putnbr.c \
		  $(UTILS)ft_putnbr_fd.c \
		  $(UTILS)ft_putstr.c \
		  $(UTILS)ft_putstr_fd.c \
		  $(UTILS)get_next_line.c \
		  $(UTILS)gnl_newline.c

INCLUDES = $(addprefix $(INCLUDES_FOLDER), libft.h ft_printf.h get_next_line.h)
OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(SOURCES:.c=.o))

GREEN = \033[38;2;12;231;58m
RED = \033[38;2;255;60;51m
YELLOW = \033[38;2;251;196;15m
RMLINE = \033[2K
RESET = \033[0m
HIDE = tput civis
SHOW = tput cnorm
SLEEP = sleep 0.01

ifneq (,$(filter $(flags),n no))
	CCFLAGS =
endif

ifneq (,$(filter $(fsanitize),y yes))
	CCFLAGS += -g3
	CCFLAGS += -fsanitize=address
endif

ifneq (,$(filter $(silent), y yes))
	HIDE :=
	SLEEP :=
	REDIRECT := > /dev/null
endif

all: $(NAME) Makefile

$(NAME): $(OBJECTS) Makefile
	$(SHOW)
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(AR) $@ $(OBJECTS)
	printf "$(GREEN)$@ has been created$(RESET)\n" $(REDIRECT)
	$(RLIB) $@
	printf "$(GREEN)$@ has been indexed$(RESET)\n" $(REDIRECT)

so: CFLAGS += -fPIC
so: $(NAME_SHARED)

$(NAME_SHARED): $(OBJECTS) Makefile
	$(SHOW)
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(CC) -shared $(OBJECTS) -o $@
	printf "$(GREEN)$@ has been created$(RESET)\n" $(REDIRECT)

objects/%.o: %.c $(INCLUDES) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDES_FOLDER) -o $@ -c $<
	$(HIDE)
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(SLEEP)

clean:
	$(RM) $(OBJECTS_FOLDER)
	printf "$(RED)The libft objects have been removed$(RESET)\n" $(REDIRECT)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_SHARED)
	printf "$(RED)$(NAME) has been removed$(RESET)\n" $(REDIRECT)

re: fclean all

.PHONY: all clean fclean re so

.SILENT: $(NAME) $(OBJECTS) clean fclean $(NAME_SHARED)
