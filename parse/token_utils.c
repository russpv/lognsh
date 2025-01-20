#include "token.h"

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
