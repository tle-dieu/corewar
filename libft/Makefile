# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-dieu <tle-dieu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/18 01:10:03 by tle-dieu          #+#    #+#              #
#    Updated: 2019/07/24 16:27:00 by tle-dieu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
NAME_DYNAMIC = libft.so
MAKEFLAGS += --no-print-directory

CC ?= gcc
CFLAGS = -Wall -Werror -Wextra
DCFLAGS ?= -fPIC
DEPFLAGS = -MMD -MF $(<:$(SOURCES_DIR)%.c=$(ACTUAL_DEPS_DIR)%.d)

AR = ar rcs
RM = rm -rf

# ------------------- Sources ------------------ #

SOURCES_DIR = sources/
SOURCES = $(addprefix $(SOURCES_DIR), $(SOURCES_FILES:.c=.o))

FT_PRINTF = ft_printf/
STRING = string/
NUMERIC = numeric/
MEMORY = memory/
LIST = list/
UTILS = utils/


SOURCES_FILES =	$(FT_PRINTF)buff.c \
				$(FT_PRINTF)colors.c \
				$(FT_PRINTF)conv_cs.c \
				$(FT_PRINTF)conv_di.c \
				$(FT_PRINTF)conv_f.c \
				$(FT_PRINTF)conv_lcls.c \
				$(FT_PRINTF)conv_oux.c \
				$(FT_PRINTF)conv_p.c \
				$(FT_PRINTF)flags.c \
				$(FT_PRINTF)ftoa_binary.c \
				$(FT_PRINTF)ftoa_bint.c \
				$(FT_PRINTF)ftoa_calc.c \
				$(FT_PRINTF)ftoa_list.c \
				$(FT_PRINTF)format.c \
				$(FT_PRINTF)ft_dprintf.c \
				$(FT_PRINTF)ft_printf.c \
				$(FT_PRINTF)ft_sprintf.c \
				$(FT_PRINTF)utils.c \
				$(STRING)ft_count_occ.c \
				$(STRING)ft_ncount_occ.c \
				$(STRING)ft_count_words.c \
				$(STRING)ft_count_words_reject.c \
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
				$(STRING)ft_strnjoin.c \
				$(STRING)ft_strlcat.c \
				$(STRING)ft_strlen.c \
				$(STRING)ft_strlwr.c \
				$(STRING)ft_strmap.c \
				$(STRING)ft_strmapi.c \
				$(STRING)ft_strmode.c \
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
				$(STRING)ft_strsplit_reject.c \
				$(STRING)ft_strspn.c \
				$(STRING)ft_strstr.c \
				$(STRING)ft_strsub.c \
				$(STRING)ft_strtrim.c \
				$(STRING)ft_strupr.c \
				$(STRING)ft_tolower.c \
				$(STRING)ft_toupper.c \
				$(NUMERIC)ft_atoi.c \
				$(NUMERIC)atoi_flag.c \
				$(NUMERIC)ft_atoll.c \
				$(NUMERIC)atoui_flag.c \
				$(NUMERIC)ft_isalnum.c \
				$(NUMERIC)ft_isalpha.c \
				$(NUMERIC)ft_isascii.c \
				$(NUMERIC)ft_isdigit.c \
				$(NUMERIC)ft_isprint.c \
				$(NUMERIC)ft_iterative_power.c \
				$(NUMERIC)ft_itoa.c \
				$(NUMERIC)ft_sort_integer_table.c \
				$(NUMERIC)ft_sqrt.c \
				$(NUMERIC)swap_bytes.c \
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


# ------------------- Objects ------------------ #

OBJECTS_DIR = objects/
STATIC_OBJECTS_DIR = $(OBJECTS_DIR)static/
DYNAMIC_OBJECTS_DIR = $(OBJECTS_DIR)dynamic/

STATIC_OBJECTS = $(addprefix $(STATIC_OBJECTS_DIR), $(SOURCES_FILES:.c=.o))
DYNAMIC_OBJECTS = $(addprefix $(DYNAMIC_OBJECTS_DIR), $(SOURCES_FILES:.c=.o))


# ------------------ Includes ------------------ #

INCLUDES_DIR = includes/


# ---------------- Dependencies ---------------- #

DEPS_DIR = .d/
STATIC_DEPS_DIR = $(DEPS_DIR)static/
DYNAMIC_DEPS_DIR = $(DEPS_DIR)dynamic/


# ---------------- Directories ----------------- #

STATIC_DIRS = $(sort $(dir $(STATIC_OBJECTS)))
STATIC_DIRS += $(sort $(dir $(addprefix $(STATIC_DEPS_DIR), $(SOURCES_FILES))))
DYNAMIC_DIRS = $(sort $(dir $(DYNAMIC_OBJECTS)))
DYNAMIC_DIRS += $(sort $(dir $(addprefix $(DYNAMIC_DEPS_DIR), $(SOURCES_FILES))))
DIRS = $(sort $(DYNAMIC_DIRS) $(STATIC_DIRS))


# -------------------- Style ------------------- #

GREEN = \033[38;2;12;231;58m
RED = \033[38;2;255;60;51m
YELLOW = \033[38;2;251;196;15m
RMLINE = \033[2K
RESET = \033[0m

HIDE = tput civis
SHOW = tput cnorm

# ------------------- Options ------------------ #

ifneq (,$(filter $(fsanitize),y yes))
	CFLAGS += -g3
	CC = clang
ifeq ($(shell uname -s),Linux)
	CFLAGS += -fsanitize=address,undefined,integer,bounds,builtin
else
	CFLAGS += -fsanitize=address,undefined,integer,bounds
endif
endif

ifneq (,$(filter $(silent), y yes))
	HIDE :=
	REDIRECT := > /dev/null 2>&1
endif


# -------------------- Rules ------------------- #

all:
	$(MAKE) -j $(NAME)

so:
	$(MAKE) -j $(NAME_DYNAMIC)

$(NAME): ACTUAL_DEPS_DIR = $(STATIC_DEPS_DIR)
$(NAME): $(STATIC_OBJECTS) Makefile
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(SHOW)
	$(AR) $@ $(STATIC_OBJECTS)
	printf "$(GREEN)$@ has been created$(RESET)\n" $(REDIRECT)


$(NAME_DYNAMIC): ACTUAL_DEPS_DIR = $(DYNAMIC_DEPS_DIR)
$(NAME_DYNAMIC): $(DYNAMIC_OBJECTS) Makefile
	printf "$(RMLINE)$(YELLOW)🌘  All compiled$(RESET)\n" $(REDIRECT)
	$(SHOW)
	$(CC) $(CFLAGS) -shared $(DYNAMIC_OBJECTS) -o $@
	printf "$(GREEN)$@ has been created$(RESET)\n" $(REDIRECT)

$(STATIC_OBJECTS_DIR)%.o: $(SOURCES_DIR)%.c Makefile | $(STATIC_DIRS)
	$(HIDE)
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(CC) $(CFLAGS) $(DEPFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

$(DYNAMIC_OBJECTS_DIR)%.o: $(SOURCES_DIR)%.c Makefile | $(DYNAMIC_DIRS)
	$(HIDE)
	printf "$(RMLINE)\r🚀 $(GREEN)$(YELLOW) Compiling:$(RESET) $(notdir $<)\r" $(REDIRECT)
	$(CC) $(CFLAGS) $(DCFLAGS) $(DEPFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

$(DIRS):
	mkdir -p $@

clean:
	$(RM) $(OBJECTS_DIR) $(DEPS_DIR)
	printf "$(RED)The libft objects have been removed$(RESET)\n" $(REDIRECT)

fclean: clean
	$(RM) $(NAME) $(NAME_DYNAMIC)
	printf "$(RED)$(NAME) has been removed$(RESET)\n" $(REDIRECT)

re: fclean all

-include $(addprefix $(DYNAMIC_DEPS_DIR), $(SOURCES_FILES:.c=.d)) $(addprefix $(STATIC_DEPS_DIR), $(SOURCES_FILES:.c=.d))

.PHONY: all clean fclean re so

.SILENT: all so $(NAME) $(NAME_DYNAMIC) $(STATIC_OBJECTS) $(DYNAMIC_OBJECTS) clean fclean $(DIRS)
