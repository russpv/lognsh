
#include "parse_int.h"

// Can only replace/update the grp node with the combined word
int	do_redir_inserts(t_state *s, t_list **this_node)
{
	t_list			**tok_lst;
	t_redir_data	*r;

	tok_lst = get_tmp_tok_list(s);
	r = (*this_node)->content;
	r->filename = tok_get_raw((*tok_lst)->content);
	ft_lstprinter(*this_node, print_redir_tok);
	*tok_lst = NULL;
	lgprint("Redir inserts done.\n");
	return (0);
}
