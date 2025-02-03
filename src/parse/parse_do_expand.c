#include "parse_int.h"

/* Add special $? codes here */
int	_check_special_expansions(t_arg_data *data, const char *buf, char **value)
{
		const int *stat = get_status(data->global_state);

	if (!buf || !value)
		return (-1);
	if (ft_strcmp(buf, "?") == 0)
	{
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

/* Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in ENV and replaces
 * t_arg_data.raw string, omitting '$'.
 */
void	p_do_expansion(void *c)
{
	char		buf[1024];
	char		*value;
	t_arg_data	*content;
	size_t		raw_len;
	char		*new_raw;

	value = NULL;
	ft_memset(buf, 0, sizeof(buf));
	content = (t_arg_data *)c;
	debug_print("p_do_expansion got: %s\n", content->raw);
	if (content->do_expansion)
	{
		raw_len = ft_strnlen(content->raw, 1024);
		if (raw_len <= 1)
			return ;
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
				new_raw = ft_strdup(value);
				if (!new_raw)
					return ;
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
char	**p_do_arg_processing(t_ast_node *a)
{
	t_list	*args;
	char	**tmp;

	if (a->type != AST_NODE_CMD)
		return (NULL);
	args = p_get_args(a);
	ft_lstiter(args, p_do_expansion);
	ft_lstiter(args, p_do_globbing); // TODO
	tmp = list_to_array(args, a->data.cmd.argc);
	return (tmp);
}

/* Does modification in place or throws error,
 * which will prevent execution.
 * (No need to free orig_fn, command cleanup will get it)
 */
int	p_do_redir_processing(t_ast_node *a)
{
	t_list	*redirs;
	char *orig_fn;

	if (a->type != AST_NODE_CMD)
		return (-1);
	redirs = p_get_redirs(a);
	ft_lstiter(redirs, p_do_expansion);
	orig_fn = ft_lstiterstr(redirs, p_do_globbing_redirs);
	if (NULL != orig_fn)
	{
		print_ambiguous_redirect(((t_redir_data *)orig_fn)->filename);
		return (-1);
	}
	return (0);
}
