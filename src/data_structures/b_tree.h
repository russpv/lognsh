#ifndef BIN_TREE_H
# define BIN_TREE_H

# include <stdlib.h>
/* BINARY TREE ADT
 */

/* Forward declaration (opaque pointer) */
typedef	struct	s_btnode	*t_btree;

/* Methods */
t_btree	btree_create_node(int value);
int		destroy_btree(t_btree bt);
void	btree_traverse(t_btree bt, void (*visit)(void *));

#endif
