#include "parse_int.h"

bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_ENV_VAR || type == TOK_EXIT_STATUS);
}

void	parse_print(t_ast_node *ast)
{
	(void)ast;
	// TODO call printer methods
	// ast_traverse
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
			destroy_arr(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}
