
#include "parse_int.h"

/* Moves ptr and returns previous object */
t_tok	*advance(t_parser *p)
{
	t_tok	*tmp;

	tmp = p->curr_tok->content;
	if (false == is_at_end(p))
	{
		p->curr_idx++;
		p->curr_tok = p->curr_tok->next;
	}
	return (tmp);
}

// Returns current token ptr token
t_tok	*peek(t_parser *p)
{
	return (p->curr_tok->content);
}

// Returns p->curr_tok->next->content
t_tok	*lookahead(t_parser *p)
{
	if (is_at_end(p))
		return (NULL);
	else
		return (p->curr_tok->next->content);
}

t_tok	*prev(t_parser *p)
{
	return (p->curr_tok->prev->content);
}
