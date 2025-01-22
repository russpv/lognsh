#include "token_int.h"

void	tok_print(void *content)
{
	t_tok	*token;

	token = (t_tok *)content;
	if (token)
	{
		log_printf("Token: %d Exp: %d Value: %s \n", token->type,
			token->do_expansion, token->raw);
	}
}

char	*tok_get_raw(t_tok *token)
{
	return (token->raw);
}

enum e_tok_type	tok_get_type(t_tok *token)
{
	return(token->type);
}
