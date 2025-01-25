#include "llist.h"

// Helper function: Copy a single node's content
t_list	*ft_lstcopy_node(const t_list *orig, void *(*f)(const void *))
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = f(orig->content);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

// f - content copy func; d - content delete func
t_list	*ft_lstcopy(t_list *orig, void *(*f)(const void *),
		void *(*d)(const void *))
{
	t_list	*new_list;
	t_list	*curr;
	t_list	*new_node;
	t_list	*prev_node;

	new_list = NULL;
	curr = orig;
	prev_node = NULL;
	while (curr != NULL)
	{
		new_node = ft_lstcopy_node(curr, f);
		if (!new_node)
		{
			ft_lstclear(new_list, d);
			return (NULL);
		}	
		if (prev_node != NULL)
			prev_node->next = new_node;
		else
			new_list = new_node;
		prev_node = new_node;
		curr = curr->next;
	}
	return (new_list);
}
