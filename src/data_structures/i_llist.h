#ifndef I_LLIST_H
# define I_LLIST_H

// Adapter for LLIST to add memory management.

# include "../../include/libft.h"
# include "llist.h"
# include <assert.h>
# include <stdio.h>

typedef struct s_mem_mgr
{
	t_mem_node		list;
	t_alloc_fn		f;
	t_dealloc_fn	dealloc;
}					t_mem_mgr;

t_list				*ft_lstnew_tmp(t_mem_mgr *m, void *content);
void				ft_lstdelone_tmp(t_mem_mgr *s, t_list **lst, t_list *node,
						void (*del)(t_mem_mgr *m, void *));
void				ft_lstclear_tmp(t_mem_mgr *mgr, t_list **lst,
						void (*del)(t_mem_mgr *m, void *));

t_list				*ft_lstcopy_tmp(t_mem_mgr *mgr, t_list *orig,
						void *(*f)(t_mem_mgr *s, const void *),
						void (*del)(t_mem_mgr *, void *));

t_list				*ft_lstcopy_node_tmp(t_mem_mgr *mgr, const t_list *orig,
						void *(*f)(t_mem_mgr *s, const void *));

void				ft_lstdelone_rwd_tmp(t_mem_mgr *mgr, t_list **lst,
						t_list **node, void (*del)(t_mem_mgr *, void *));

t_list				*ft_lstmap_tmp(t_mem_mgr *mgr, t_list *lst,
						void *(*f)(t_mem_mgr *, void *),
						void (*del)(t_mem_mgr *, void *));

void				ft_lstiter_ins_rwd_tmp(t_mem_mgr *mgr, t_list **lst,
						int (*f)(t_mem_mgr *, t_list **, void *));

char				*ft_strdup_tmp(t_mem_mgr *mgr, const char *s);

#endif