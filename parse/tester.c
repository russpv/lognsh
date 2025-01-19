#include <stdio.h>

int main(void)
{
	const char *input = "echo \"Hello, world\" > file.txt && cat file.txt";
	t_lex *lexer = tokenize(input);
	lexer_print(lexer);
	destroy_lexer(lexer);
	return (0);
}
