#include "parse_int.h"

bool	is_expansion(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_EXIT_STATUS || type == TOK_ENV_VAR);
}

/* If flag set, looks for expansion values in ENV and replaces 
 * t_arg_data.raw string 
 * Applied to t_ast_node.cmd's t_list*
 */
void	p_do_expansion(void *c)
{
	char buf[1024];

	ft_memset(buf, 0, 1024);
	t_arg_data *content = (t_arg_data *)c;
	debug_print("p_do_expansion got: %s\n", content->raw);
	if (content->do_expansion)
	{
		ft_memcpy(buf, content->raw + 1, ft_strnlen(content->raw, 1024) - 1);
		const char *value = getenv(buf);
		debug_print("p_do_expansion found: %s\n", value);
		if (value)
		{
			free(content->raw);
			content->raw = ft_strdup(value);
			if (!content->raw)
				err("p_do_expansion Malloc\n");
		}
	}
}

/* Needs to work with the arg llist 
 * and mind the flags
 */
char **p_do_arg_expansions(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	t_list *args = p_get_args(a);
	ft_lstiter(args, p_do_expansion);
	//char **tmp = p_get_argv(a);
	char **tmp = list_to_array(args, a->data.cmd.argc);
	return (tmp);
}
