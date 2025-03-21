#include "lex_int.h"

/* Returns a new data node */
t_ht_data	lex_create_ht_node(t_mem_mgr *m, int is_substring, int type)
{
	t_ht_data	data;

	data = myalloc(&m->list, sizeof(struct s_ht_data));
	if (data)
	{
		data->is_substring = is_substring;
		data->type = type;
	}
	else
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (data);
}

/* Destroys t_ht_data data, defined in this module */
void	lex_destroy_ht_node(t_mem_node *n, void *node)
{
	myfree(n, node);
}

void	lex_print(t_lex *lexer)
{
	if (NULL == lexer)
		return ;
	if (NULL == lexer->token_list)
		return ;
	ft_lstiter(lexer->token_list, tok_print);
}

/* for ht_install this will return a new copy of the argument
 * But we don't need to use this
 */
void	*lex_copy_ht_data(t_mem_node *head, void *data)
{
	t_ht_data	newdata;

	newdata = myalloc(head, sizeof(struct s_ht_data));
	if (newdata)
	{
		newdata->is_substring = ((t_ht_data)data)->is_substring;
		newdata->type = ((t_ht_data)data)->type;
	}
	else
		exit_clean(head, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (newdata);
}
