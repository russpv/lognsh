CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -lreadline -lncurses -fsanitize=address  # Add ASAN for linking

SRC = ../data_structures/llist.c  ../data_structures/llist_utils.c \
	  ../data_structures/hashtable.c  ../data_structures/hashtable_utils.c \
      ../../lib/ft_strchr.c ../../lib/ft_strdup.c ../../lib/ft_isalpha.c ../../lib/ft_isdigit.c \
      ../../lib/ft_memcpy.c ../../lib/ft_strlen.c ../../lib/ft_strncmp.c ../../lib/ft_memset.c ../../lib/ft_isspace.c \
	  ../../lib/ft_isalnum.c \
      ../../debug.c \
	  lex_utils.c lex_normal_utils.c lex_normal_ht.c lex_squote.c lex_dquote.c lex_utils_ht.c \
	  lex_state.c lex_build_ht.c lex_.c lex_normal.c lex_heredoc.c \
	  ../token/token.c ../token/token_utils.c \
	  tester.c

OBJ = $(SRC:.c=.o)
NAME = test_lexer

# The main build target
$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Clean up object files
clean:
	rm -f $(OBJ)

# Clean everything, including the binary
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean $(NAME)
