#include <stdio.h>
#include "lex.h"

int main(void)
{
const char *inputs[] = {
    // Existing Test Cases
    "echo \"Hello, world\" > file.txt && cat file.txt",
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
    "if [ -f file.txt ]; then echo \"File exists\"; else echo \"File not found\"; fi",
    "echo \"The current date is $(date)\"",
    "echo $HOME",
    "echo \"The path is \\\"C:\\\\Program Files\\\\\"\"",
    "cat << EOF\nHello\nWorld\nEOF",
    "while [ \"$1\" -gt 0 ]; do echo \"Processing\"; done"
};

    // Number of inputs to process
    int num_inputs = sizeof(inputs) / sizeof(inputs[0]);

    // Loop through each input string
    for (int i = 0; i < num_inputs; i++) {
        const char *input = inputs[i];  // Get the current input string

        // Print the input to be processed
        printf("Processing input %d: %s\n", i + 1, input);
        fflush(stdout);  // Ensure output is printed immediately

        // Tokenize and process the input
        t_lex *lexer = tokenize(input);  // Assuming tokenize function returns a lexer

        if (lexer)
        {
            lex_print(lexer);                 // Print tokens (assuming lex_print prints the tokens)
            fflush(stdout);
           destroy_lexer(lexer);             // Clean up after processing the lexer
        }
        printf("\n");  // Print a newline between test cases for readability
    }

    return 0;
}
