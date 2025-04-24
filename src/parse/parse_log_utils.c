
#include "parse_int.h"

t_ast_node	*init_log(t_mem_mgr *m)
{
	t_ast_node	*log_node;

	log_node = m->f(&m->list, sizeof(t_ast_node));
	if (log_node)
	{
		log_node->type = AST_NODE_LOG;
		log_node->data.log.cmds = NULL;
		log_node->data.log.cmdc = 0;
		log_node->data.log.operators = NULL;
	}
	return (log_node);
}

/* Returns cmp if it is a lower priority operator than
 * that represented by ref. Note: should never be comparing 
 * log node to log op token.
 */
t_tok	*which_lower_priority(t_ast_node *ref, t_tok *cmp)
{
	if (!ref | !cmp)
		return ((void*)-1);
	if (is_log_token(cmp) && AST_NODE_LOG == ref->type)
		return ((void*)-1); 
	if (is_close_paren(cmp) || tok_get_type(cmp) == TOK_EOF)
		return (cmp);
	if (is_group_op_token(cmp))
	{
		if (ref->type == AST_NODE_PIPELINE && is_log_token(cmp))
			return (cmp);
	}
	return (NULL);
}
