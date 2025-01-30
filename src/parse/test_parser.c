#include <stdio.h>
#include "../lex/lex.h"
#include "parse_int.h"  // Include your parser header

void print_ast(t_ast_node *ast, int depth) {
    // You can implement an AST printer here to print the nodes for debugging.
    if (!ast) return;
    // Indentation based on depth for readability
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    switch (ast->type) {
        case AST_NODE_CMD:
            printf("Command: %s", ast->data.cmd.name);
            
            // Print arguments (if any)
            if (ast->data.cmd.argc > 0) {
                printf("  Arguments: ");
                for (t_list *arg_node = ast->data.cmd.args; arg_node != NULL; arg_node = arg_node->next) 
                {
                    t_arg_data *arg = (t_arg_data *)arg_node->content;
                    printf("%s", arg->raw);

                    // Print additional information about the argument
                    if (arg->option) {
                        printf(" (option)");
                    }
                    if (arg->do_globbing) {
                        printf(" (glob)");
                    }
                    if (arg->do_expansion) {
                        printf(" (expand)");
                    }

                    printf(" ");
                }
            }

            // Print redirects (if any)
            if (ast->data.cmd.redc > 0) {

                printf("  Redirects: ");
                for (t_list *redir_node = ast->data.cmd.redirs; redir_node != NULL; redir_node = redir_node->next) {
                    t_redir_data *redir = (t_redir_data *)redir_node->content;
                    printf("%d|%s %s ", redir->type, redir->symbol, redir->filename);
                }
            }
            printf("\n");
            break;
        case AST_NODE_PIPELINE:
            printf("Pipeline with %d commands:\n", ast->data.pipe.cmdc);
            for (t_list *cmd_node = ast->data.pipe.cmds; cmd_node != NULL; cmd_node = cmd_node->next) {
                print_ast(cmd_node->content, depth + 1);
            }
            break;
        case AST_NODE_LOG:
            printf("Logical with %d commands:\n", ast->data.log.cmdc);
            for (t_list *cmd_node = ast->data.log.cmds; cmd_node != NULL; cmd_node = cmd_node->next) {
                print_ast(cmd_node->content, depth + 1);
            }
            break;
        case AST_NODE_PROC:
            printf("Proc () with %d commands:\n", ast->data.proc.cmdc);
            for (t_list *cmd_node = ast->data.proc.cmds; cmd_node != NULL; cmd_node = cmd_node->next) {
                print_ast(cmd_node->content, depth + 1);
            }
            break;
        // Add cases for other node types
        default:
            printf("Unknown AST Node\n");
            break;
    }
}

int main(void) {
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
        "if [ -f file.txt ]; then echo \"File exists\"; else echo \"File not found\"; fi",
        "echo \"The current date is $(date)\"",
        "echo $HOME",
        "echo \"The path is \\\"C:\\\\Program Files\\\\\\\"\"",
        "cat << EOF",
        "while [ \"$1\" -gt 0 ]; do echo \"Processing\"; done",

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
        "asdf"
    };

    // Number of inputs to process
    int num_inputs = sizeof(inputs) / sizeof(inputs[0]);

    for (int i = 0; i < num_inputs; i++) {
        const char *input = inputs[i];

        printf("Processing input %d: %s\n", i + 1, input);
        fflush(stdout);

        // Tokenize the input
        t_lex *lexer = tokenize(input); 
        if (lexer) {
            // Print lexed tokens for debugging
            lex_print(lexer);
            fflush(stdout);

            // Now parse the tokens into an AST
            t_ast_node *ast = test_parse(create_parser(lex_get_tokens(lexer)));
            if (ast) {
                printf("PRINTING AST NODES:\n");

                // Print the AST for verification
                print_ast(ast, 5);
            } else {
                printf("Parsing failed for input %d\n", i + 1);
            }

            // Clean up lexer and AST
            destroy_lexer(lexer);
        } else {
            printf("Tokenization failed for input %d\n", i + 1);
        }

        printf("\n");
    }

    return 0;
}
