#ifndef LLIST_INT_H
# define LLIST_INT_H

# include "../utils/debug.h"
# include "llist.h"

# define ALESSTHANB -1
# define AMORETHANB 1
# define MAX_BUF 1024

struct		s_merge
{
	t_list	*l;
	t_list	*r;
	t_list	**tail;
	t_list *tmp;  // need this to set node->prev
	t_list *tmp2; // need this to reconnect segment with rest of list
	t_list	*mid;
	t_list	**beg;
	t_list	**end;
	int		nodec;
	int		i;
};

void		init_merge(struct s_merge *m, t_list **beg, t_list **end,
				t_list *mid);

void		merge(t_list **lst, t_list **beg, t_list *mid, t_list **end);
int			compare(t_list *beg, t_list *end);

void		print_addremainder_post(struct s_merge *m);
void		print_tailadvance_post(struct s_merge *m);
void		print_tailnext_post(struct s_merge *m);
void		print_swap_post(t_list **lst, struct s_merge *m);
void		print_prev_pre(struct s_merge *m);
void		print_prev_post(struct s_merge *m);
void		print_diagnostics_end(t_list **lst, struct s_merge *m);
void		print_diagnostics_input(t_list **lst, struct s_merge *m);
void		debug_print_tail_info(struct s_merge *m);

bool		str_hasalnum(const char *s);
bool		str_isalnum(const char *s);

#endif