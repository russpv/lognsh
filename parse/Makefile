CC = cc
CFLAGS = -Wall -Wextra -Werror -g -DDEBUGMODE -fsanitize=address
SRC = lex_.c lex_normal.c token.c tester.c ../data_structures/llist.c ../data_structures/hashtable.c \
      ../lib/ft_isalnum.c ../lib/ft_strchr.c ../lib/ft_strdup.c ../lib/ft_isalpha.c ../lib/ft_isdigit.c \
      ../lib/ft_memcpy.c ../lib/ft_strlen.c ../lib/ft_strncmp.c ../lib/ft_memset.c \
      ../debug.c lex_utils.c lex_normal_utils.c lex_normal_ht.c lex_squote.c lex_dquote.c lex_utils_ht.c \
	  lex_state.c lex_build_ht.c token_utils.c
OBJ = $(SRC:.c=.o)
NAME = lexer

# The main build target
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up object files
clean:
	rm -f $(OBJ)

# Clean everything, including the binary
fclean: clean

# Rebuild everything
re: fclean all
