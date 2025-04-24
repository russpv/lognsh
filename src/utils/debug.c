
#include "../data_structures/llist.h"
#include "debug.h"

void	debug_detect_cycle(t_list *head)
{
	t_list	*slow;
	t_list	*fast;

	if (!head)
		return ;
	if (DEBUG)
	{
		slow = head;
		fast = head;
		while (fast && fast->next)
		{
			slow = slow->next;
			fast = fast->next->next;
			if (slow == fast)
			{
				err("Cycle detected in the linked list\n");
				return ;
			}
		}
		dvprint("No cycle detected in the linked list\n");
	}
}

void	print_array(char *arr[])
{
	int	i;

	i = 0;
	if (DEBUGVERBOSE)
	{
		if (!arr)
			return ;
		while (arr[i] != NULL)
		{
			fprintf(stderr, "%s\n", arr[i]);
			i++;
		}
	}
}

bool	test_prev_integrity(t_list *lst)
{
	t_list	*current;
	t_list	*previous;

	current = lst;
	if (current == NULL)
		return (dvprint(YELLOW, "List is empty.\n"), true);
	previous = NULL;
	while (current != NULL)
	{
		if (current->prev != previous)
			return (cprintf(YELLOW,
					"Check fail: Node at %p->prev is incorrect.\n", current),
				false);
		if (previous != NULL && previous->next != current)
			return (cprintf(YELLOW,
					"Check fail: Node at %p->next is incorrect.\n", previous),
				false);
		previous = current;
		current = current->next;
	}
	return (dvprint(YELLOW, "Integrity check passed.\n"), true);
}
