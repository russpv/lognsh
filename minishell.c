#include "minishell.h"

int g_exit_code;


# define PROMPT "minish> "

int main() {
	// TODO t_state s = init_state();
    // signal(SIGINT, sigint_handler); // Ctrl-C
	// signal(SIG..., ..._handler); // Ctrl-...
    char *input = NULL;
    t_ast_node *ast;
    t_state *s;

    s = init_state();
    while (1) {
        input = readline(PROMPT);
        
        if (input == NULL) {
            // User pressed Ctrl+D or another exit condition
            break;
        }
		
        // If input is not empty, add it to history
        if (input[0] != '\0') {
            add_history(input);
        }
		
	    ast = parse(s, input); //TODO: t_ast ast = parse(s, input);
		cmd_execute_simple(ast); //TODO: s.status = cmd_execute(s, ast);

		//parse_free_ast(ast);
        free(input);
    }
	//TODO destroy_state(s);
    return 0;
}

