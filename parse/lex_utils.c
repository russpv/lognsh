#include "lex_int.h"

/* add to llist tail a new token, clear buf */
int	add_token(t_lex *lexer, t_tok *token)
{
	debug_print( "add_token\n");
	if (token && lexer)
	{
		memset(lexer->buf, 0, LEX_BUFSZ);
		ft_lstadd_back(&lexer->token_list, ft_lstnew(token));
		return (0);
	}
	return (1);
}

/* Creates token based on current buf and ptr, does not add to llist */
t_tok	*lex_create_token(t_lex *lexer, int type)
{
	debug_print( "lex_create_token\n");
	if (!lexer)
		return (NULL);
	t_tok *token = create_token(lexer->buf, type, (size_t)(lexer->ptr - lexer->raw_string));
	if (token)
	{
		if (DO_GLOBBING == lexer->do_globbing)
			tok_set_globbing(token);
		if (DO_EXPANSION == lexer->do_expansion)
			tok_set_expansion(token);
		lexer->do_globbing = RESET;
		lexer->do_expansion = RESET;
	}
	return (token);
}

t_ht_data	lex_create_ht_node(int is_substring, int type)
{
	t_ht_data data = malloc(sizeof(struct s_ht_data));
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

/* Somewhat dumb function, currently */
void *lex_copy_ht_data(void *data)
{
	t_ht_data newdata = malloc(sizeof(struct s_ht_data));
	if (newdata)	
	{
		newdata->is_substring = ((t_ht_data)data)->is_substring;
		newdata->type = ((t_ht_data)data)->type;
	}
	return (newdata);
}

void	lex_print(t_lex *lexer)
{
	if (NULL == lexer)
		return ;
	if (NULL == lexer->token_list)
		return ;
	ft_lstiter(lexer->token_list, tok_print);
}

void debug_detect_cycle(t_list *head) {
	if (!head)
		return ;
    t_list *slow = head;
    t_list *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            printf("Cycle detected in the linked list\n");
            return;
        }
    }
    printf("No cycle detected in the linked list\n");
}