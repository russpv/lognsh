#include "lex_int.h"

t_ht_data	lex_create_ht_node(int is_substring, int type)
{
	t_ht_data	data;

	data = malloc(sizeof(struct s_ht_data));
	if (data)
	{
		data->is_substring = is_substring;
		data->type = type;
	}
	return (data);
}

void	lex_destroy_ht_node(void *node)
{
	free((t_ht_data)node);
}

void	lex_print(t_lex *lexer)
{
	if (NULL == lexer)
		return ;
	if (NULL == lexer->token_list)
		return ;
	ft_lstiter(lexer->token_list, tok_print);
}

/* for ht_install */
void	*lex_copy_ht_data(void *data)
{
	t_ht_data	newdata;

	newdata = malloc(sizeof(struct s_ht_data));
	if (newdata)
	{
		newdata->is_substring = ((t_ht_data)data)->is_substring;
		newdata->type = ((t_ht_data)data)->type;
	}
	return (newdata);
}
