#include "parse_int.h"

void	do_grparg_inserts(t_state *s, t_list **this_node)
{
	t_list	**tok_lst;
	t_list	*argl;

	tok_lst = get_tmp_tok_list(s);
	argl = ft_lstmap_tmp(get_mem(s), *tok_lst, token_to_arg, destroy_arg);
	ft_lstadd_insert(this_node, argl);
	ft_lstdelone_tmp(get_mem(s), this_node, *this_node, destroy_arg);
	ft_lstprinter(*this_node, print_arg);
	*tok_lst = NULL;
}
