CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRC = lex.c tester.c
OBJ = $(SRC:.c=.o)
NAME = lexer

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
