NAME = libft.a
SOURCES = ft_memcpy.c \
		  ft_memset.c \
		  ft_bzero.c \
		  ft_memmove.c \
		  ft_memchr.c \
		  ft_memcmp.c \
		  ft_calloc.c \
		  ft_strlen.c \
		  ft_toupper.c \
		  ft_tolower.c \
		  ft_isalpha.c \
		  ft_isdigit.c \
		  ft_isalnum.c \
		  ft_isspace.c \
		  ft_isascii.c \
		  ft_strlcpy.c \
		  ft_strlcat.c \
		  ft_strncmp.c \
		  ft_strchr.c \
		  ft_strrchr.c \
		  ft_strnstr.c \
		  ft_isprint.c \
		  ft_strjoin.c \
		  ft_strjoin_free.c \
		  ft_strtrim.c \
		  ft_substr.c \
		  ft_strnlen.c \
		  ft_itoa_mem.c \
		  ft_atoi.c \
		  ft_putchar_fd.c \
		  ft_putendl_fd.c \
		  ft_putnbr_fd.c \
		  ft_putstr_fd.c \
		  ft_split_mem.c \
		  ft_split.c \
		  ft_strmapi.c \
		  ft_striteri.c \
		  ft_strdup.c \
		  ft_strndup.c \
		  ft_freearr_mem.c \
		  ft_strcmp.c \
		  ft_strncat.c \
		  ft_strcpy.c \
		  ft_strcat.c \
		  ft_putstr.c \
		  ft_putnbr.c \
		  ft_putendl.c \
		  ft_putchar.c \
		  ft_print_memory.c \
		  ft_memccpy.c \
		  ft_strncpy.c \
		  ft_strscpy.c \
		  ft_strndup.c \
		  ft_strcmplow.c

OBJECTS = $(SOURCES:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS)
	$(AR) -r $@ $?

%.o: %.c
	$(CC) -c $(CFLAGS) $?

bonus:

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

so:
	$(CC) -fPIC $(CFLAGS) -c $(OBJECTS) 
	$(CC) -shared -o libft.so $(OBJECTS)


.PHONY: all clean fclean re so
