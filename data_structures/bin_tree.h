#ifndef BIN_TREE_H
# define BIN_TREE_H

/* ************* */
/* BINARY TREE ADT
 * Gonna implement a basic tree of nodes */

/* Forward declaration (opaque pointer) */
typedef	struct	s_btnode	*t_btree;

/* Methods */
t_btree	btree_create_node(int value);
int	destroy_btree(t_btree bt);
// TODO some sort of traversal


// TODO put this in separate file
struct	s_btnode {
	void	*payload; // token.c, 
	struct s_btnode	*left;
	struct s_btnode *right;
}

#endif
