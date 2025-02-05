#ifndef LLIST_H
# define LLIST_H

/* DOUBLY LINKED LIST */

# include "../debug.h"
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
void					ft_lstdelone(t_list **lst, t_list *node,
							void (*del)(void *));
void					ft_lstiter(t_list *lst, void (*f)(void *));
char					*ft_lstiterstr(t_list *lst, int (*f)(void *));

t_list					*ft_lstlast(t_list *lst);
t_list					*ft_lstmap(t_list *lst, void *(*f)(void *),
							void (*del)(void *));
t_list					*ft_lstnew(void *content);
int						ft_lstsize(t_list *lst);
t_list					*ft_lstcopy(t_list *orig, void *(*f)(const void *),
							void (*d)(void *));
t_list					*ft_lstcopy_node(const t_list *orig,
							void *(*f)(const void *));

void					ft_lstiter_ins_rwd(t_list **lst, void (*f)(t_list **,
								void *));
void					ft_lstadd_insert(t_list **lst, t_list *new);
void					ft_lstdelone_rwd(t_list **lst, t_list **node,
							void (*del)(void *));

#endif
