#include "parse_int.h"

/* True if token type needs to be expanded.
 */
bool	is_expansion(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_EXIT_STATUS || type == TOK_ENV_VAR);
}

/* Add special $? codes here */
int _check_special_expansions(t_arg_data *data, const char *buf, char **value)
{
	if (!buf || !value)
		return (-1);
	if (ft_strcmp(buf, "?") == 0)
	{
		const int *stat = get_status(data->global_state);
		if (stat == NULL)
			*value = ft_itoa(0);
		else
			*value = ft_itoa(*stat);
		if (*value == NULL)
			return (-1); 
		debug_print("_check_special_expansions got: %s\n", *value);
		return (0);
	}
	return (1);
}


/* Applied to t_ast_node.cmd's t_list*.
 * If flag set, looks for expansion values in ENV and replaces 
 * t_arg_data.raw string, omitting '$'.
 */
void	p_do_expansion(void *c)
{
	char buf[1024];
	char *value = NULL;

	ft_memset(buf, 0, sizeof(buf));
	t_arg_data *content = (t_arg_data *)c;
	debug_print("p_do_expansion got: %s\n", content->raw);
	if (content->do_expansion)
	{
		size_t raw_len = ft_strnlen(content->raw, 1024);
		if (raw_len <= 1)
			return;
		ft_memcpy(buf, content->raw + 1, raw_len - 1);
		if (0 == _check_special_expansions(content, buf, &value))
		{
			if (value)
			{
				free(content->raw);
				content->raw = value;
			}
		}
		else
		{
			value = getenv(buf);
			if (value)
			{
				char *new_raw = ft_strdup(value);
				if (!new_raw)
					return;
				free(content->raw);
				content->raw = new_raw;
			}
		}
		debug_print("p_do_expansion found: %s\n", value);
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
	char **tmp = list_to_array(args, a->data.cmd.argc);
	return (tmp);
}
