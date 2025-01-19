#include <stdio.h>
#include "lex.h"

int main(void)
{
	const char *input = "echo \"Hello, world\" > file.txt && cat file.txt";
	t_lex *lexer = tokenize(input);
	lex_print(lexer);
	destroy_lexer(lexer);
	return (0);
}
