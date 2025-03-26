#ifndef LIBFT_H
# define LIBFT_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_mem_node
{
	struct s_mem_node		*next;
	struct s_mem_node		*prev;
}							t_mem_node;

typedef struct s_mem_block
{
	t_mem_node				node;
	size_t					size;
	bool					is_freed;
	void					*payload;
}							t_mem_block;

/* Memory allocator utils */
typedef struct s_mem_node	t_mem_node;
typedef void				*(*t_alloc_fn)(t_mem_node *head, size_t s);
typedef void				(*t_dealloc_fn)(t_mem_node *head, void *alloc);

struct						s_mem_utils
{
	t_alloc_fn				f;
	t_dealloc_fn			u;
	t_mem_node				*head;
};

typedef enum e_bool
{
	FALSE,
	TRUE,
}							t_bool;
typedef unsigned char t_uint8_t; /* 1 byte */
typedef unsigned int t_uint32_t; /* 4 bytes */
typedef struct s_16
{
	t_uint8_t				data[16];
} t_block_16; /* 16 bytes */
typedef struct s_32
{
	t_uint8_t				data[32];
} t_block_32; /* 32 bytes */

typedef unsigned long	long(t_uint64_t);  /* Must be at least 8-bytes */
typedef unsigned long	long(t_uintptr_t); /* 64-bit ptr */

# define BIGBLOCKSZ 32
# define BLOCK 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFULL /* 16-bytes */
# define BLOCKSZ 16
# define SUBBLOCK 0xFFFFFFFFFFFFFFFFUL /* 8 bytes */
# define SUBBLOCKSZ 8
# define PTRSZ 8 /* 8-byte ptr */
# define ULSZ 4

/*
 * Extras / TODO
 */

size_t						ft_strnlen(const char *s, size_t maxlen);
char						*ft_strcpy(char *dest, const char *src);
char						*ft_strncpy(char *dest, const char *src, size_t n);
size_t						ft_strscpy(char *dest, const char *src,
								size_t count);
char						*ft_strcat(char *dest, const char *src);
char						*ft_strncat(char *dest, const char *src, size_t n);
void						*ft_memccpy(void *dest, const void *src, int c,
								size_t n);
int							ft_strcmp(const char *s1, const char *s2);
void						ft_freearr(void **arr, int size);
size_t						strscpy(char *dest, const char *src, size_t count);

/*
** Print functions
*/

void						ft_putchar(char c);
void						ft_putstr(char const *s);
void						ft_putendl(char const *s);
void						ft_putnbr(int n);
void						*ft_print_memory(void *addr, unsigned int size);

void						ft_putchar_fd(char c, int fd);
void						ft_putstr_fd(char const *s, int fd);
void						ft_putendl_fd(char const *s, int fd);
void						ft_putnbr_fd(int n, int fd);

/*
** String functions
*/

size_t						ft_strlen(const char *s);
char						*ft_strdup(const char *src);
char						*ft_strndup(const char *s, size_t size);
size_t						ft_strlcpy(char *dest, const char *src, size_t n);
size_t						ft_strlcat(char *dest, const char *src, size_t n);
char						*ft_strchr(const char *str, int ch);
char						*ft_strchrs(const char *raw, const char *chars);
char						*ft_strrchr(const char *str, int ch);
char						*ft_strnstr(const char *big, const char *little,
								size_t len);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
int							ft_toupper(int c);
int							ft_tolower(int c);
void						ft_striteri(char *s, void (*f)(unsigned int,
									char *));
char						*ft_strmapi(char const *s, char (*f)(unsigned int,
									char));
char						*ft_substr(char const *s, unsigned int start,
								size_t len);
char						*ft_strjoin(char const *s1, char const *s2);
char						*ft_strjoin_free(char *s1, const char *s2);
char						*ft_strtrim(char const *s1, char const *set);
char						**ft_split(char const *s, char c);

/*
** Memory manipulation
*/

void						*ft_memset(void *str, int c, size_t n);
void						ft_bzero(void *s, size_t n);
void						*ft_memcpy(void *dest, const void *src, size_t n);
void						*ft_memmove(void *dest, const void *src, size_t n);
void						*ft_memchr(const void *ptr, int ch, size_t n);
int							ft_memcmp(const void *lhs, const void *rhs,
								size_t n);
void						*ft_calloc(size_t nelem, size_t elsize);

/*
** Tests
*/

t_bool						ft_isalpha(int c);
t_bool						ft_isdigit(int c);
t_bool						ft_isalnum(int c);
t_bool						ft_isascii(int c);
t_bool						ft_isprint(int c);
t_bool						ft_isspace(int c);

/*
** Conversions
*/

int							ft_atoi(const char *str);
char						*ft_itoa_mem(t_mem_node *head, t_alloc_fn f,
								int num);
void						ft_freearr_mem(t_mem_node *head,
								t_dealloc_fn dealloc, void **arr, int size);
char						*ft_strjoin_mem(t_mem_node *head, t_alloc_fn f,
								char const *s1, char const *s2);
char						**ft_split_mem(struct s_mem_utils *m, char const *s,
								char c);
char	**ft_split_ifs_mem(struct s_mem_utils *m, char const *s, char const *set);
char	*ft_strtrim_mem(struct s_mem_utils *m, char const *s1, char const *set);

int							ft_strcmp_low(const char *s1, const char *s2);
int							ft_strcmp_casefold(const char *s1, const char *s2);
char	*ft_skip_delims(char const *s, char const *ref);

char	*ft_strtrimback_mem(struct s_mem_utils *m, char const *s1, char const *set);
char	*ft_strtrimfront_mem(struct s_mem_utils *m, char const *s1, char const *set);
int	ft_is_set(char const *s1, char const *set);
char	*ft_strchrs_betw(const char *start, const char *end, const char *chars);

#endif
