/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:58:08 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:58:09 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	debugv_print(const char *format, ...)
{
	va_list	args;

	if (DEBUGVERBOSE && isatty(STDERR_FILENO))
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
	t_list	*current;
	t_list	*previous;

	current = lst;
	if (current == NULL)
		return (colored_printf(YELLOW, "List is empty.\n"), true);
	previous = NULL;
	while (current != NULL)
	{
		if (current->prev != previous)
		{
			colored_printf(YELLOW, "Integrity check failed: Node at\
				%p's prev pointer is incorrect.\n", current);
			return (false);
		}
		if (previous != NULL && previous->next != current)
		{
			colored_printf(YELLOW, "Integrity check failed: Node at\
				%p's next pointer is incorrect.\n", previous);
			return (false);
		}
		previous = current;
		current = current->next;
	}
	colored_printf(YELLOW, "Integrity check passed.\n");
	return (true);
}
