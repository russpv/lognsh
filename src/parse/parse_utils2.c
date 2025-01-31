#include "parse_int.h"

bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_ENV_VAR || type == TOK_EXIT_STATUS);
}

void print_ast(t_ast_node *ast, int depth) 
{
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


static void	_destroy_arr(char **arr)
{
	int i = 0;
	if (!arr[i])
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// Deep copies linked list of arguments to char **array
char	**list_to_array(t_list *args, int argc)
{
	char	**array;
	t_list	*current;
	int		i;

	array = malloc(sizeof(char *) * (argc + 1));
	if (!array)
		return (NULL);
	current = args;
	i = 0;
	while (i < argc && current)
	{
		array[i] = ft_strdup(((t_arg_data *)current->content)->raw);
		if (!array[i])
		{
			_destroy_arr(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}
