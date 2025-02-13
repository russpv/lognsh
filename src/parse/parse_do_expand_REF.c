#include "parse_int.h"

/* Custom llist iterator with t_state function ptr */
static void	_lstiter_state(t_state *s, t_list *lst, void (*f)(t_state *,
			void *))
{
	if (lst == NULL)
		return ;
	while (lst)
	{
		f(s, (lst)->content);
		lst = (lst)->next;
	}
}

/* Add special $? codes here */
static int	_check_special_expansions(t_arg_data *data, const char *buf,
		char **value)
{
	const int	*stat = get_status(data->global_state);

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
		debug_print("Parser: _check_special_expansions got: %s\n", *value);
		return (0);
	}
	return (ERR_GENERAL);
}

/* Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in ENV and replaces
 * t_arg_data.raw string, omitting '$'.
 */
static void	p_do_expansion(t_state *s, void *c)
{
	char		buf[MAX_INPUT];
	char		*value;
	t_arg_data	*content;
	size_t		raw_len;
	char		*new_raw;

	value = NULL;
	ft_memset(buf, 0, sizeof(buf));
	content = (t_arg_data *)c;
	debug_print("Parser: p_do_expansion got: %s\n", content->raw);
	if (content->do_expansion)
	{
		raw_len = ft_strnlen(content->raw, MAX_INPUT);
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
			value = get_sh_env(s, buf);
			if (value)
			{
				new_raw = ft_strdup(value);
				if (!new_raw)
					return ;
				free(content->raw);
				content->raw = new_raw;
			}
		}
		debug_print("Parser: p_do_expansion found: %s\n", value);
	}
}

/* Needs to work with the arg llist
 * and mind the flags
 */
char	**p_do_arg_processing(t_state *s, t_ast_node *a)
{
	t_list	**args;
	char	**tmp;

	if (a->type != AST_NODE_CMD)
		return (NULL);
	args = p_get_args(a);
	_lstiter_state(s, *args, p_do_expansion);
	// TODO test that this does not corrupt the args list
	debug_print("Parser: Before globbing: head: %p, first node: %p\n", args,
		*args ? (*args)->content : NULL);
	ft_lstiter_ins_rwd(args, p_do_globbing);
	debug_print("Parser: After globbing: head: %p, first node: %p\n", args,
		*args ? (*args)->content : NULL);
	a->data.cmd.argc = ft_lstsize(*args);
	tmp = list_to_array(*args, a->data.cmd.argc);
	return (tmp);
}

/* Does modification in place or throws error,
 * which will prevent execution.
 * (No need to free orig_fn, command cleanup will get it)
 */
int	p_do_redir_processing(t_state *s, t_ast_node *a)
{
	t_list	*redirs;
	char	*orig_filen;

	if (a->type != AST_NODE_CMD)
		return (-1);
	redirs = p_get_redirs(a);
	_lstiter_state(s, redirs, p_do_expansion);
	debug_print("Parser: p_do_redir_processing...\n");
	orig_filen = ft_lstiterstr(redirs, p_do_globbing_redirs);
	if (NULL != orig_filen)
	{
		print_ambiguous_redirect(((t_redir_data *)orig_filen)->filename);
		return (-1);
	}
	return (0);
}
