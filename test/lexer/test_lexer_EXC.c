#include <stdio.h>
#include "lex.h"

int main(void)
{

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
    "(echo \"Hello\")",
    "echo \"Hello, World!\" # This is a comment",
    "echo \"Hello\" && ls -la || cat file.txt",
    "grep \"Hello\" | awk '{count++} END {print count}' > output.txt",
    "if [ -f file.txt ]; then echo \"File exists\"; else echo \"File not found\"; fi",
    "echo \"The current date is $(date)\"",
    "echo $HOME",
    "echo \"The path is \\\"C:\\\\Program Files\\\\\\\"\"",
    "cat << EOF\nHello\nWorld\nEOF",
    "while [ \"$1\" -gt 0 ]; do echo \"Processing\"; done",

    // Heredoc cases
    "<< EOF Test stuff \n stuff EOF",

    // Absolute path 
    "/bin/ls",

    // Builtin cases
    "echo \"haha\"",
    };

    // Number of inputs to process
    int num_inputs = sizeof(inputs) / sizeof(inputs[0]);

    for (int i = 0; i < num_inputs; i++) {
        const char *input = inputs[i];

        printf("Processing input %d: %s\n", i + 1, input);
        fflush(stdout); 
        t_lex *lexer = tokenize(input); 
        if (lexer)
        {
            lex_print(lexer);
            fflush(stdout);
            destroy_lexer(lexer); 
        }
        printf("\n");
    }

    return 0;
}
