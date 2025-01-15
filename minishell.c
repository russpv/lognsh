#include "minishell.h"

int g_exit_code;

char **parse(const char *input)
{
	int n = 0;
	char **res = malloc(n * sizeof(char*));
	return (res);
}

int	execute(char **args)
{
	printf("%s", *args);
	return (1);
}

int main() {
    // signal(SIGINT, sigint_handler); // Ctrl-C
	// signal(SIG..., ..._handler); // Ctrl-...
	// Set up a prompt string
    char *input = NULL;

    // Loop to read input until user exits
    while (1) {
        // Use readline to get user input
        input = readline("Enter a command: ");
        
        if (input == NULL) {
            // User pressed Ctrl+D or another exit condition
            break;
        }
		
        // If input is not empty, add it to history
        if (input[0] != '\0') {
            add_history(input);
        }
		
		// Run parser
		char **args = parse(input);

        // Execute
		int exit_code = execute(args);

        free(input);
    }

    return 0;
}

