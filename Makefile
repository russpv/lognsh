CC = cc
CFLAGS = -Wall -Wextra -Werror -g -DDEBUGMODE
LDFLAGS = -lreadline -lncurses  # Add ASAN for linking -fsanitize=address 

SRC = src/data_structures/llist.c src/data_structures/llist_utils.c src/data_structures/llist_cpy.c \
      src/data_structures/hashtable.c src/data_structures/hashtable_utils.c \
      src/data_structures/stack.c \
      lib/ft_strchr.c lib/ft_strdup.c lib/ft_isalpha.c lib/ft_isdigit.c lib/ft_strncmp.c \
      lib/ft_memcpy.c lib/ft_strlen.c lib/ft_memset.c lib/ft_isspace.c lib/ft_strcmp.c \
      lib/ft_isalnum.c lib/ft_strnlen.c lib/ft_freearr.c lib/ft_split.c lib/ft_strjoin.c \
      lib/ft_memmove.c lib/ft_isprint.c lib/ft_itoa.c lib/ft_atoi.c \
      debug.c \
      src/lex/lex_utils.c src/lex/lex_normal_utils.c src/lex/lex_normal_ht.c src/lex/lex_squote.c src/lex/lex_dquote.c src/lex/lex_utils_ht.c \
      src/lex/lex_state.c src/lex/lex_build_ht.c src/lex/lex_.c src/lex/lex_normal.c src/lex/lex_heredoc.c \
      src/token/token.c src/token/token_utils.c \
      src/parse/parse.c src/parse/parse_stack.c src/parse/parse_utils.c src/parse/parse_class.c src/parse/parse_cmd.c \
      src/parse/parse_redir.c src/parse/parse_log.c src/parse/parse_proc.c src/parse/parse_pipe.c \
      src/parse/parse_bools.c src/parse/parse_bools2.c src/parse/parse_utils2.c src/parse/parse_getters.c \
      src/parse/parse_do_expand.c src/parse/parse_do_redir.c src/parse/parse_setters.c \
      src/state/error.c src/state/state.c src/state/env.c src/state/state_setters.c src/state/state_getters.c src/state/state_callbacks.c\
      src/command/command.c src/command/cmd_getters.c src/command/cmd_simple.c \
      src/execute/execute.c src/execute/exec_redirect.c src/execute/exec_simple.c\
	src/builtins/bi_echo.c src/builtins/bi_pwd.c src/builtins/bi.c src/builtins/bi_exit.c \
      minishell.c

OBJ = $(SRC:.c=.o)
NAME = main

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
