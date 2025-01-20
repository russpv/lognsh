#include "token.h"

t_tok	*create_token(const char *s, int type, size_t pos)
{
	if (!s)
		return (NULL);
	debug_print("create_token: %s_\n", s);
	t_tok *token = malloc(sizeof(t_tok));
	if (token)
	{
		token->type = type;
		token->raw = ft_strdup(s);
		token->pos = pos;
		token->do_expansion = RESET;
		token->do_globbing = RESET;
		if (!token->raw)
		{
			free(token);
			token = NULL;
		}
	}
	return (token);
}

void *copy_token_data(void *data) 
{
    t_tok *token = (t_tok *)data;
    t_tok *new_token = malloc(sizeof(t_tok));
    if (new_token == NULL) 
		return NULL;

    new_token->raw = strdup(token->raw);
    new_token->type = token->type;
	new_token->pos = token->pos;
    new_token->do_globbing = token->do_globbing;
    new_token->do_expansion = token->do_expansion;

    return new_token; // Return the new token
}

int	tok_set_globbing(t_tok *token)
{
	token->do_globbing = DO_GLOBBING;
	return (0);
}

int	tok_set_expansion(t_tok *token)
{
	token->do_expansion = DO_EXPANSION;
	return (0);
}

/* void star due to linked list destroy method */
void	destroy_token(void *token)
{
	if (!token)
		return ;
	if (((t_tok *)token)->raw)
		free(((t_tok *)token)->raw);
	free((t_tok *)token);
	token = NULL;
}

void tok_print(void *content)
{
	t_tok *token = (t_tok *)content;  // Cast content to the correct type
	if (token) 
	{
		log_printf("Token: %d Exp: %d Value: %s \n", token->type, token->do_expansion, token->raw);  // Print the value of the token
	}
}
