#include "parse_int.h"

#define EMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define EMSG_NULL_EXPAN "Null expansion variable.\n"

// Processes any group args first, then normal args
static int	_helper(t_state *s, t_ast_node *a, t_list **argl)
{
	int	res;

	res = 0;
	if (a->data.cmd.has_arggrouptoks)
		res = ft_lstiter_state_ins_rwd_mem(s, argl, p_do_grparg_processing);
	if (a->data.cmd.do_expansion)
		res = lstiter_state(s, *argl, p_do_arg_expansion);
	if (a->data.cmd.do_expansion && a->data.cmd.do_wordsplit)
		ft_lstiter_ins_rwd_tmp(get_mem(s), argl, p_do_wordsplits);
	if (a->data.cmd.do_globbing)
		ft_lstiter_ins_rwd_tmp(get_mem(s), argl, p_do_globbing_args);
	dprintdiv("\n--- Arg Processing Done ---\n\n");
	return (res);
}

/* PROCESS ARGS
 *
 * Does expansions: Shell parameters, word splitting,
 * filename expansions (glob*) as needed.
 * Then converts to array and returns that array via args ptr.
 * Note: we traverse llists backwards to avoid inserted nodes
 * Note: procs return early, since no args.
 */
int	p_process_args(t_state *s, t_ast_node *a, char ***args)
{
	t_list	**argl;
	int		res;

	res = 0;
	if (a->type == AST_NODE_PROC)
		return (0);
	if (a->type != AST_NODE_CMD)
		return (err("Invalid node type"), ERR_INVALID_CMD_TYPE);
	argl = p_get_args(a);
	if (*argl)
	{
		res = _helper(s, a, argl);
		if (0 != res)
			return (res);
		a->data.cmd.argc = ft_lstsize(*argl);
		res = lstiter_state_rwd_trim(s, argl, p_is_arg_null, destroy_arg);
		if (res < 0)
			a->data.cmd.argc += res;
		dprint_arglist(*argl);
		*args = list_to_array(get_mem(s), *argl, a->data.cmd.argc);
		if (NULL == *args)
			return (ERR_MEM);
	}
	return (0);
}


/* -------------------------------------------------------------------------
 * Function Group: Group Token Argument Logic
 * Description   : Handles dispatching of group token command arguments
 *                 during command execution.
 * ------------------------------------------------------------------------- */

void	do_grparg_inserts(t_state *s, t_list **this_node)
{
	t_list	**tok_lst;
	t_list	*argl;

	tok_lst = get_tmp_tok_list(s);
	argl = ft_lstmap_tmp(get_mem(s), *tok_lst, token_to_arg, destroy_arg);
	ft_lstadd_insert(this_node, argl);
	ft_lstdelone_tmp(get_mem(s), this_node, *this_node, destroy_arg);
	ft_lstprinter(*this_node, print_arg);
	*tok_lst = NULL;
}

// Modifies args token list
static int	_do_expansions(t_state *s, t_arg_data *grparg)
{
	int	res;

	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_expansion)
	{
		res = lstiter_state(s, grparg->lst_tokens, tok_do_expansion);
		if (0 != res)
			return (res);
		tok_print_list(grparg->lst_tokens);
	}
	lgprint("Expansions Done.\n");
	return (0);
}

// Modifies args token list
static int	_do_wordsplits(t_state *s, t_arg_data *grparg)
{
	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_expansion && !grparg->in_dquotes)
	{
		ft_lstiter_ins_rwd_tmp(get_mem(s), &grparg->lst_tokens,
			tok_do_wordsplits);
		tok_print_list(grparg->lst_tokens);
	}
	lgprint("Wordsplits Done.\n");
	return (0);
}

// Saves modified token list in state cache
static int	_do_combine(t_state *s, t_arg_data *grparg)
{
	int		res;
	char	*str;
	t_list	**tok_lst;

	res = lstiter_state(s, grparg->lst_tokens, tok_do_grp_combine);
	if (0 != res)
		return (err("grp tok aggregation failure\n"), res);
	tok_lst = get_tmp_tok_list(s);
	str = get_tmp(s);
	if (str)
	{
		if (*str)
			ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s),
					create_tmp_token(get_mem(s), str)));
		tok_print_list(*tok_lst);
	}
	lgprint("Combines and Inserts Done.\n");
	return (0);
}

// Saves list in state
static int	_do_globbing(t_state *s, t_arg_data *grparg)
{
	t_list	**saved_lst;

	if (!grparg || !s)
		return (ERR_ARGS);
	saved_lst = get_tmp_tok_list(s);
	if (grparg->do_globbing || grparg->do_expansion)
	{
		ft_lstiter_ins_rwd_tmp(get_mem(s), saved_lst, p_do_globbing_toks);
	}
	lgprint("Globbing Done.\n");
	return (0);
}

/* Passed to arg llist iterator to iterate any group arg's token llist.
 * If (null) results, still returns empty heap string,
 * since a grouparg implies non-null word.
 * Ignores all (null) token raws.
 * Inserts into arg llist as needed.
 * expand > split > glob > combine > insert
 * // Explicit empty strings retained as empty strings.
// Unquoted nulls resulting from expansions are removed.
// Quoted nulls resulting from expansions are retained as empty strings,
//	(done prior)
// unless part of a non-null expansion word. -d'' is -d
 */
int	p_do_grparg_processing(t_state *s, t_list **this_arg, void *c)
{
	t_arg_data	*grparg;
	int			res;

	grparg = (t_arg_data *)c;
	if (NULL == c || false == grparg->is_grouparg || NULL == grparg->lst_tokens)
		return (0);
	dvprint(_MOD_ ": %s: got list: %p exp:%d glob:%d\n", __FUNCTION__,
		grparg->lst_tokens, grparg->do_expansion, grparg->do_globbing);
	res = _do_expansions(s, grparg);
	if (0 != res)
		return (res);
	res = _do_wordsplits(s, grparg);
	if (0 != res)
		return (res);
	res = _do_combine(s, grparg);
	if (0 != res)
		return (res);
	res = _do_globbing(s, grparg);
	if (0 != res)
		return (res);
	do_grparg_inserts(s, this_arg);
	dprint("%s: returning lst_size: %d, %p\n", __FUNCTION__,
		ft_lstsize(*this_arg), *this_arg);
	return (res);
}
