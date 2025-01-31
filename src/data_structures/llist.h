#ifndef LLIST_H
# define LLIST_H

/* DOUBLY LINKED LIST */

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_list
{
	void				*content;
	size_t				content_size;
	struct s_list		*next;
	struct s_list		*prev;
}						t_list;

typedef struct s_list	t_list;

void					ft_lstadd_back(t_list **lst, t_list *new);
void					ft_lstadd_front(t_list **lst, t_list *new);
void					ft_lstclear(t_list **lst, void (*del)(void *));
void					ft_lstdelone(t_list **lst, t_list *node, void (*del)(void *));
void					ft_lstiter(t_list *lst, void (*f)(void *));
t_list					*ft_lstlast(t_list *lst);
t_list					*ft_lstmap(t_list *lst, void *(*f)(void *), \
							void (*del)(void *));
t_list					*ft_lstnew(void *content);
int						ft_lstsize(t_list *lst);
t_list					*ft_lstcopy(t_list *orig, void *(*f)(const void *), \
							void (*d)(void *));
t_list					*ft_lstcopy_node(const t_list *orig, \
							void *(*f)(const void *));

/*
t_ll					ll_create(void);
//ll_clear(t_ll ll); //destroy all nodes except head
ll_destroy(t_ll ll);

ll_insert_head(t_ll ll, void *data);
ll_insert_tail(t_ll ll, void *data);
//ll_insert_idx(t_ll ll, void *data, size_t pos);
ll_remove_head(t_ll ll);
//ll_remove_tail(t_ll ll);
//ll_remove_idx(t_ll ll, size_t pos);

ll_get_firstn(t_ll ll);
ll_get_lastn(t_ll ll);
ll_get_value();
//ll_get_index();
//ll_get_size(t_ll ll);
//ll_is_empty(t_ll ll);

//ll_update(t_ll ll, size_t pos);
//ll_iter_do(t_ll ll, (*action)(void *));
*/
#endif
