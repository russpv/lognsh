#include "token_int.h"

t_tok	*create_token(const char *s, int type, size_t pos)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	debug_print("Token: create_token, init'g: %s_ t_%d \n", s, type);
	token = malloc(sizeof(t_tok));
	if (token)
	{
		token->type = type;
		token->raw = ft_strdup(s);
		token->pos = pos;
		token->do_expansion = false;
		token->do_globbing = false;
		if (!token->raw)
		{
			free(token);
			token = NULL;
		}
	}
	return (token);
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

void	*copy_token(const void *tok)
{
	t_tok	*token;
	t_tok	*new_token;

	token = (t_tok *)tok;
	new_token = malloc(sizeof(t_tok));
	if (new_token == NULL)
		return (NULL);
	new_token->raw = ft_strdup(token->raw);
	new_token->type = token->type;
	new_token->pos = token->pos;
	new_token->do_globbing = token->do_globbing;
	new_token->do_expansion = token->do_expansion;
	return (new_token);
}

int	tok_set_globbing(t_tok *token)
{
	token->do_globbing = true;
	debug_print("Token: tok_set_globbing: %s_ glob_%d \n", tok_get_raw(token),
		tok_get_globbing(token));
	return (0);
}

int	tok_set_dqoute(t_tok *token)
{
	token->in_dquotes = true;
	return (0);
}

int	tok_set_expansion(t_tok *token)
{
	token->do_expansion = true;
	debug_print("Token: tok_set_expansion: %s_ glob_%d \n", tok_get_raw(token),
		tok_get_expansion(token));
	return (0);
}

bool	tok_get_globbing(t_tok *token)
{
	return (token->do_globbing);
}

bool	tok_get_expansion(t_tok *token)
{
	return (token->do_expansion);
}

bool	tok_get_dquotes(t_tok *token)
{
	return (token->in_dquotes);
}

int	tok_set_dquotes(t_tok *token)
{
	token->in_dquotes = true;
	return (0);
}