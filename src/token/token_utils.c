#include "token_int.h"

int	tok_print(void *content)
{
	t_tok	*tok;

	tok = (t_tok *)content;
	if (tok)
	{
		if (GROUP == tok->class)
		{
			lgprint(_MOD_ ": Grp_tok:(grp) Exp:- Glb:- Sub:- tokc:%d \n",
				tok->t.meta.tokc);
			ft_lstiter(tok->t.meta.tokens, tok_print);
		}
		else if (tok_get_issubtoken(tok))
			lgprint(_MOD_": Stok:%02d Exp:%d Glb:%d Sub:%d Dq:%d +:%d Val:(%s)\
				 \n", tok->t.tok.type, tok->t.tok.do_expansion,
				tok->t.tok.do_globbing, tok->t.tok.is_subtoken,
				tok->t.tok.in_dquotes, tok->t.tok.is_combinable,
				tok->t.tok.raw);
		else
			lgprint(_MOD_": Tok:%02d Exp:%d Glb:%d Sub:%d Dq:%d +:%d Val:(%s)\
				\n", tok->t.tok.type, tok->t.tok.do_expansion,
				tok->t.tok.do_globbing, tok->t.tok.is_subtoken,
				tok->t.tok.in_dquotes, tok->t.tok.is_combinable,
				tok->t.tok.raw);
	}
	return (0);
}

void	tok_print_list(t_list *head)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		while (current)
		{
			dprint("Node %d:\n", i);
			tok_print(current->content);
			current = current->next;
			i++;
		}
		dprint("List printed.\n");
	}
}
