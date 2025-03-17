#include "debug.h"
#include "../data_structures/llist.h"

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


void print_array(char *arr[]) 
{
    int i = 0;
	if (DEBUG)
	{	
		while (arr[i] != NULL) 
		{
			printf("%s\n", arr[i]);
			i++;
		}
	}
}
