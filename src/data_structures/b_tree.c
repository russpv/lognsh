#include "b_tree.h"

struct	s_btnode {
	void	*payload; 
	struct s_btnode	*left;
	struct s_btnode *right;
};

t_btree	btree_create_node(int value)
{
	(void)value;
	return (NULL);
	//TODO
}

int		destroy_btree(t_btree bt)
{
	(void)bt;
	return (0);
	//TODO
}

// visit function does something to node
void	btree_traverse(t_btree bt, void (*visit)(void *))
{
	(void)bt;
	(void)visit;
	//TODO
}

