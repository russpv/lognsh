#include "token.h"

t_tok	*create_token(const char *s, int type, size_t pos)
{
	if (!s)
		return (NULL);
	t_tok *token = malloc(sizeof(t_tok));
	if (token)
	{
		token->type = type;
		token->raw = ft_strdup(s);
		token->pos = pos;
		if (!token->raw)
		{
			free(token);
			token = NULL;
		}
	}
	return (token);
}

int	tok_set_globbing(t_tok *token)
{
	token->do_globbing = DO_GLOBBING;
	return (0);
}

int	tok_set_expansion(t_tok *token)
{
	token->do_expansion(DO_EXPANSION);
	return (0);
}

/* void star due to linked list destroy method */
void	destroy_token(void *token)
{
	if (!token)
		return ;
	free(token->raw);
	free(token);
	token = NULL;
}

void tok_print(void *content)
{
    t_tok *token = (t_tok *)content;  // Cast content to the correct type
    if (token) {
        printf("Token: %d Value: %s\n", token->type, token->raw);  // Print the value of the token
    }
}
