CC = cc
CFLAGS = -Wall -Wextra -Werror -g -DDEBUGMODE
LDFLAGS = -lreadline -lncurses -fsanitize=address  # Add ASAN for linking

SRC = ../data_structures/llist.c ../data_structures/llist_utils.c ../data_structures/llist_cpy.c \
      ../data_structures/hashtable.c ../data_structures/hashtable_utils.c \
      ../data_structures/stack.c \
      ../../lib/ft_strchr.c ../../lib/ft_strdup.c ../../lib/ft_isalpha.c ../../lib/ft_isdigit.c ../../lib/ft_strncmp.c \
      ../../lib/ft_memcpy.c ../../lib/ft_strlen.c ../../lib/ft_memset.c ../../lib/ft_isspace.c ../../lib/ft_strcmp.c \
      ../../lib/ft_isalnum.c \
      ../../debug.c \
      ../lex/lex_utils.c ../lex/lex_normal_utils.c ../lex/lex_normal_ht.c ../lex/lex_squote.c ../lex/lex_dquote.c ../lex/lex_utils_ht.c \
      ../lex/lex_state.c ../lex/lex_build_ht.c ../lex/lex_.c ../lex/lex_normal.c ../lex/lex_heredoc.c \
      ../token/token.c ../token/token_utils.c \
      ../parse/parse.c ../parse/parse_stack.c ../parse/parse_utils.c ../parse/parse_class.c ../parse/parse_cmd.c \
      ../parse/parse_redir.c ../parse/parse_log.c ../parse/parse_proc.c ../parse/parse_pipe.c \
      ../parse/parse_bools.c ../parse/parse_bools2.c ../parse/parse_utils2.c ../parse/parse_getters.c \
      ../state/error.c ../state/state.c \
      ../command/command.c \
      ../execute/execute.c \
	../builtins/bi_echo.c ../builtins/bi_pwd.c ../builtins/bi.c \
      test_parser.c

OBJ = $(SRC:.c=.o)
NAME = test_parser

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
