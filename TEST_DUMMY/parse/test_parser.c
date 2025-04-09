#include "../lex/lex.h"
#include "parse_int.h" // Include your parser header
#include <stdio.h>

int	main(void)
{
	int			num_inputs;
	t_lex		*lexer;
	t_ast_node	*ast;

	const char *inputs[] = {
        // Existing Test Cases
        "(echo \"Hello, world\" > file.txt) && cat file.txt",
        "grep Hello | wc -l",
        "grep Hello && wc -l",
        "grep Hello | ls -la src/",
        "ls -la src/ | wc -l",
        "grep Hello | awk '{count++} END {print count}'",
        "grep Hello | awk \"{count++} END {print count}\"",
        "grep Hello | awk '\"{count++} END {print count}\"'",
        "grep Hello | awk \"'{count++} END {print count}'\"",
        // Additional Test Cases
        "echo \"Hello\"",
        "echo \"Hello, World!\" # This is a comment",
        "echo \"Hello\" && ls -la || cat file.txt",
        "grep \"Hello\" | awk '{count++} END {print count}' > output.txt",
        "if [-f file.txt ]; then echo \"File exists\"; else echo \"File not found\"; fi",
        "echo \"The current date is $(date)\"",
        "echo $HOME",
        "echo \"The path is \\\"C:\\\\Program Files\\\\\\\"\"",
        "cat << EOF",
        "while [ \"$1\"-gt 0 ]; do echo \"Processing\"; done",
        // Absolute path
        "/bin/ls",
        // Builtin cases
        "echo \"haha\"",
        "echo",
        // Grouping cases
        "(echo \"Hello\" && echo \"World\") || echo \"This should not print\"",
        // Subshell cases
        "(echo \"temporary data\" > temp.txt) && cat temp.txt",
        // Complex
        "(echo \"Success\" || echo \"Failure\") && echo \"End of chain\"",
        "(ls | grep \"log\" | wc -l) && echo \"Number of logs\"",
        "(echo \"First\" && echo \"Second\") || echo \"This should not print\" && echo \"End\"",
        "() || echo \"This should print\"",
        // Redirects
        "command > output1.txt >> output2.txt > output3.txt",
        "< input1.txt > output1.txt command < input2.txt > output2.txt",
        // Heredoc cases
        "<< EOF Test stuff",
        // Erorrs
        "asdf"};
	// Number of inputs to process
	num_inputs = sizeof(inputs) / sizeof(inputs[0]);
	for (int i = 0; i < num_inputs; i++)
	{
        const char  *input = inputs[i];
		printf("Processing input %d: %s\n", i + 1, input);
		fflush(stdout);
		// Tokenize the input
		lexer = tokenize(input);
		if (lexer)
		{
			// Print lexed tokens for debugging
			lex_print(lexer);
			fflush(stdout);
			// Now parse the tokens into an AST
			ast = test_parse(create_parser(lex_get_tokens(lexer)));
			if (ast)
			{
				printf("PRINTING AST NODES:\n");
				// Print the AST for verification
				print_ast(ast, 5);
			}
			else
			{
				printf("Parsing failed for input %d\n", i + 1);
			}
			// Clean up lexer and AST
			destroy_lexer(lexer);
		}
		else
		{
			printf("Tokenization failed for input %d\n", i + 1);
		}
		printf("\n");
	}
	return (0);
}
