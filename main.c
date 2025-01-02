#include "minishell.h"

int main() {
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

        // Print the input (for demonstration purposes)
        printf("You entered: %s\n", input);

        // Free the memory allocated by readline
        free(input);
    }

    return 0;
}

