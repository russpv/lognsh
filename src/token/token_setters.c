#include "token_int.h"

int	tok_set_globbing(t_tok *token)
{
	if (GROUP == token->class)
	{
		err(__FUNCTION__);
		exit(ERR_ARGS);
	}
	token->t.tok.do_globbing = true;
	debug_print(_MOD_": %s: %s_ glob_%d \n", __FUNCTION__, tok_get_raw(token),
		tok_get_globbing(token));
	return (0);
}

int	tok_set_expansion(t_tok *token)
{
	if (GROUP == token->class)
		token->t.meta.do_expansion = true;
	token->t.tok.do_expansion = true;
	debug_print(_MOD_": %s: %s_ exp_%d \n", __FUNCTION__, tok_get_raw(token),
		tok_get_expansion(token));
	return (0);
}

int	tok_set_dquotes(t_tok *token)
{
	if (GROUP == token->class)
	{
		err(__FUNCTION__);
		exit(ERR_ARGS);
	}
	token->t.tok.in_dquotes = true;
	return (0);
}

int	tok_set_subtoken(t_tok *token)
{
	if (GROUP == token->class)
	{
		err(__FUNCTION__);
		exit(ERR_ARGS);
	}
	token->t.tok.is_subtoken = true;
	return (0);
}

int	tok_incr_tokc(t_tok *token)
{
	if (GROUP != token->class)
	{
		err(__FUNCTION__);
		exit(ERR_ARGS);
	}
	token->t.meta.tokc++;
	fprintf(stderr, MAGENTA"Tokc now: %d\n"RESET, token->t.meta.tokc);
	return (0);
}

int	tok_add_subtok(t_tok *grp, t_tok *sub)
{
	if (NULL == grp || NULL == sub)
	{
		err(__FUNCTION__);
		exit(ERR_ARGS);
	}
	ft_lstadd_back(&grp->t.meta.tokens, ft_lstnew(sub));
	return (0);
}
