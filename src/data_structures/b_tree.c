#include "b_tree.h"

struct	s_btnode {
	void	*payload; 
	struct s_btnode	*left;
	struct s_btnode *right;
}

t_btree	btree_create_node(int value)
{
	//TODO
}

int		destroy_btree(t_btree bt)
{
	//TODO
}

// visit function does something to node
void	btree_traverse(t_btree bt, void (*visit)(void *))
{
	//TODO
}

