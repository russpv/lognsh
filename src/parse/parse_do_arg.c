#include "parse_int.h"

#define EMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define EMSG_NULL_EXPAN "Null expansion variable.\n"

#define DEBUGMSG_ARGP_PRE_G _MOD_ ": Pre-glob headp: %p, 1st node: %p\n"
#define DEBUGMSG_ARGP_POST_G _MOD_ ": Post-glob headp: %p, 1st node: %p\n"

/* Does expansions: Shell parameters, word splitting,
 * filename expansions (glob*) as needed.
 * Then converts to array and returns that array via args ptr.
 * Note: we traverse llists backwards to avoid inserted nodes
 * Note: procs return early, since no args.
 */
int	p_do_arg_processing(t_state *s, t_ast_node *a, char ***args)
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
		debug_print_list(*argl);
		if (a->data.cmd.has_arggrouptoks)
			res = ft_lstiter_state_ins_rwd_tmp(s, argl, p_do_grparg_processing);
		else
		{
			if (a->data.cmd.do_expansion)
				res = lstiter_state(s, *argl, p_do_arg_expansion);
			if (a->data.cmd.do_expansion && a->data.cmd.do_wordsplit)
				ft_lstiter_ins_rwd_tmp(get_mem(s), argl, p_do_wordsplits);
			if (a->data.cmd.do_globbing)
			{
				debug_print(DEBUGMSG_ARGP_PRE_G, argl, *argl); // TODO remove
				ft_lstiter_ins_rwd_tmp(get_mem(s), argl, p_do_globbing_args);
				debug_print(DEBUGMSG_ARGP_POST_G, argl, *argl); // TODO remove
			}
		}
		if (0 != res)
			return (res);
		a->data.cmd.argc = ft_lstsize(*argl);
		debug_print_list(*argl);
		res = lstiter_state_rwd_trim(s, argl, p_is_arg_null, destroy_arg);
		if (res < 0)
			a->data.cmd.argc += res;
		debug_print_list(*argl);
		*args = list_to_array(get_mem(s), *argl, a->data.cmd.argc);
		if (NULL == *args)
			return (ERR_MEM);
	}
	return (0);
}
