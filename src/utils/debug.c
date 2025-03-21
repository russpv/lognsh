#include "../data_structures/llist.h"
#include "debug.h"

void	debug_print(const char *format, ...)
{
	va_list	args;

	if (DEBUG && isatty(STDERR_FILENO))
	{
		va_start(args, format);
		fprintf(stderr, BLUE "[PID %d] [DEBUG] ", getpid());
		vfprintf(stderr, format, args);
		fprintf(stderr, RESET);
		va_end(args);
		fflush(stderr);
	}
}

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
		debug_print("No cycle detected in the linked list\n");
	}
}

void	print_array(char *arr[])
{
	int	i;

	i = 0;
	if (DEBUG)
	{
		while (arr[i] != NULL)
		{
			printf("%s\n", arr[i]);
			i++;
		}
	}
}

bool	test_prev_integrity(t_list *lst)
{
	t_list *current = lst;

	// If the list is empty, there's nothing to test
	if (current == NULL)
	{
		printf("List is empty.\n");
		return (true);
	}

	t_list *previous = NULL; // To track the previous node
	while (current != NULL)
	{
		// Check that the current node's prev points to the previous node
		if (current->prev != previous)
		{
			printf("Integrity check failed: Node at\
				%p's prev pointer is incorrect.\n", current);
			return (false);
		}

		// Check that the previous node's next points to the current node
		if (previous != NULL && previous->next != current)
		{
			printf("Integrity check failed: Node at\
				%p's next pointer is incorrect.\n", previous);
			return (false);
		}

		// Move to the next node
		previous = current;
		current = current->next;
	}

	// Check that the head node's prev pointer is NULL (for non-circular list)
	if (previous != NULL && previous->next == NULL && previous->prev != NULL)
	{
		printf("Integrity check failed: Tail node's prev pointer is incorrect.\n");
		return (false);
	}
	printf("Integrity check passed.\n");
	return (true);
}