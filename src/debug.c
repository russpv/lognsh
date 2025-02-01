#include "debug.h"

void	debug_print(const char *format, ...)
{
		va_list args;

	if (DEBUG)
	{
		va_start(args, format);
		fprintf(stderr, BLUE "[PID %d] ", getpid());
		vfprintf(stderr, format, args);
		fprintf(stderr, RESET);
		va_end(args);
		fflush(stderr);
	}
}
void	colored_printf(const char *color, const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	printf("%s", color);
	vprintf(format, args);
	printf(RESET);
	va_end(args);
}

void	log_printf(const char *s, ...)
{
		va_list args;

	if (LOG)
	{
		va_start(args, s);
		vfprintf(stdout, s, args);
		va_end(args);
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
				printf(RED"Cycle detected in the linked list\n"RESET);
				return ;
			}
		}
		debug_print("No cycle detected in the linked list\n");
	}
}

void	err(const char *s)
{
	fprintf(stderr, "%s", s);
}
