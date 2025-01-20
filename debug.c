#include "debug.h"

void debug_print(const char *s, ...) 
{
	if (DEBUG)
	{
    	va_list args;
    	va_start(args, s);

    	vfprintf(stderr, s, args);

    	va_end(args);
	}
}

void log_printf(const char *s, ...)
{
	if (LOG)
	{
    	va_list args;
    	va_start(args, s);

    	vfprintf(stdout, s, args);

    	va_end(args);
	}
}

void	debug_detect_cycle(t_list *head)
{
	if (!head)
		return ;
	if (DEBUG)
	{
		t_list *slow = head;
		t_list *fast = head;
		while (fast && fast->next)
		{
			slow = slow->next;
			fast = fast->next->next;
			if (slow == fast)
			{
				printf("Cycle detected in the linked list\n");
				return ;
			}
		}
		printf("No cycle detected in the linked list\n");
	}
}
