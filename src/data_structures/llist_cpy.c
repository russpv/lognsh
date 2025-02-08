#include "llist.h"

// Helper function: Deep copy a single node's content
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
	new_node->prev = NULL;
	return (new_node);
}

/* Deep copies linked lists 
 * f - content deep copy func; d - content delete func
 */
t_list	*ft_lstcopy(t_list *orig, void *(*f)(const void *), void (*d)(void *))
{
	t_list	*new_list;
	t_list	*prev_node;
	t_list	*new_node;

	new_list = NULL;
	prev_node = NULL;
	while (orig)
	{
		new_node = ft_lstcopy_node(orig, f);
		if (!new_node)
		{
			ft_lstclear(&new_list, d);
			return (NULL);
		}
		if (prev_node)
		{
			prev_node->next = new_node;
			new_node->prev = prev_node;
		}
		else
			new_list = new_node;
		prev_node = new_node;
		orig = orig->next;
	}
	return (new_list);
}
