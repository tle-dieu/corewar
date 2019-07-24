/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-dieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 10:50:07 by tle-dieu          #+#    #+#             */
/*   Updated: 2019/07/24 16:21:31 by tle-dieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <sys/types.h>

# define OSX 1
# define LINUX 2
# ifdef __linux__
#  define OS LINUX
# elif __APPLE__
# else
#  define OS 0
# endif

# define AT_LONG 1
# define AT_ONLYNUM 2
# define AT_INCREM 4
# define AT_NOOVERFLOW 8
# define AT_NOSPACE 16
# define AT_STOP 32
# define AT_NOSIGN 64
# define AT_NOERR 128

typedef struct	s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}				t_list;

/*
** ----------- STRING ------------
*/

int				ft_count_occ(char const *s, char c);
size_t			ft_count_words(char const *s, char c);
size_t			ft_count_words_reject(char const *s, char c, char *reject);
int				ft_ncount_occ(char const *s, char c, int n);
char			*ft_strcat(char *dest, const char *src);
char			*ft_strcdup(const char *s, char c);
char			*ft_strchr(const char *s, int c);
size_t			ft_strclen(char const *s, char c);
void			ft_strclr(char *s);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strcmpi(const char *s1, const char *s2);
char			*ft_strcpy(char *dest, char const *src);
size_t			ft_strcspn(const char *s, const char *reject);
void			ft_strdel(char **as);
char			*ft_strdup(const char *s);
int				ft_strequ(char const *s1, char const *s2);
void			ft_striter(char *s, void (*f)(char *));
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strjoin_sep(char const *s1, char const *s2, char c);
char			*ft_strnjoin(char const *s1, char const *s2,
				size_t n1, size_t n2);
size_t			ft_strlcat(char *dest, const char *src, size_t size);
size_t			ft_strlen(const char *s);
char			*ft_strlwr(char *str);
char			*ft_strmap(char const *s, char (*f)(char));
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void			ft_strmode(mode_t m, char *p);
char			*ft_strncat(char *dest, const char *src, size_t n);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strncpy(char *dest, char const *src, size_t n);
char			*ft_strndup(const char *s, size_t len);
int				ft_strnequ(char const *s1, char const *s2, size_t n);
char			*ft_strnew(size_t size);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_strpbrk(const char *s, const char *chr);
char			*ft_strrchr(const char *s, int c);
char			*ft_strrev(char *str);
char			**ft_strsplit(char const *s, char c);
char			**ft_strsplit_reject(char const *s, char c, char *reject);
size_t			ft_strspn(const char *s, const char *accept);
char			*ft_strstr(const char *big, const char *little);
char			*ft_strsub(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s);
char			*ft_strupr(char *str);
int				ft_tolower(int c);
int				ft_toupper(int c);

/*
** ----------- MEMORY ------------
*/

void			ft_bzero(void *s, int n);
void			*ft_memalloc(size_t size);
void			*ft_memccpy(void *dest, const void *src, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			ft_memdel(void **ap);
void			*ft_memdup(const void *src, size_t n);
void			*ft_memjoin(void const *s1, void const *s2,
				size_t n1, size_t n2);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			*ft_memset(void *s, int c, size_t n);

/*
** ----------- NUMERIC -----------
*/

int				ft_atoi(const char *str);
long long		atoi_flag(char **s, int *error, int flags, char *eos);
long long		ft_atoll(const char *nptr);
unsigned long	atoui_flag(char **s, int *error, int f, char *eos);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
int				ft_iterative_power(int nb, int power);
char			*ft_itoa(int n);
void			ft_sort_integer_table(int *tab, int size);
int				ft_sqrt(int nb);
unsigned short	swap_bytes2(unsigned short bytes);
unsigned		swap_bytes4(unsigned bytes);
unsigned long	swap_bytes8(unsigned long bytes);

/*
** ------------ LIST -------------
*/

void			ft_lstadd(t_list **alst, t_list *new);
void			ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void			ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
t_list			*ft_lstfind(t_list *begin_list, void *data_ref, int (*cmp)());
void			ft_lstiter(t_list *lst, void (*f)(t_list *elem));
int				ft_lstlen(t_list *begin_list);
t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
t_list			*ft_lstnew(void const *content, size_t content_size);

/*
** ---------- FT_PRINTF ----------
*/

int				ft_printf(char const *format, ...);
int				ft_dprintf(int fd, char const *format, ...);
int				ft_sprintf(char *str, char const *format, ...);

/*
** ------------ UTILS ------------
*/

void			ft_print_words_tables(char **tab);
void			ft_putchar(char c);
void			ft_putchar_fd(char c, int fd);
void			ft_putendl(const char *s);
void			ft_putendl_fd(const char *s, int fd);
void			ft_putnbr(int nb);
void			ft_putnbr_fd(int nb, int fd);
void			ft_putstr(const char *s);
void			ft_putstr_fd(const char *s, int fd);
int				get_next_line(const int fd, char **line);
int				gnl_newline(const int fd, char **line);

#endif
