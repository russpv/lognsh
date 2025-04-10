/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:28 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:29 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

void	ft_lstprint(t_list *lst)
{
	int	count;

	count = 0;
	if (lst == NULL)
		return ;
	if (DEBUG)
	{
		cprintf(RED, "linkedl: " RESET);
		while (lst)
		{
			count++;
			fprintf(stderr, YELLOW "_%s_ -> " RESET,
				(const char *)lst->content);
			lst = lst->next;
		}
		cprintf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

void	ft_lstprinter(t_list *lst, int (*f)(void *))
{
	int	count;

	count = 0;
	if (lst == NULL)
		return (cprintf(RED, "nothing to print.\n" RESET));
	if (DEBUG)
	{
		cprintf(RED, "linkedl: " RESET);
		while (lst)
		{
			count++;
			if (lst->content)
				f(lst->content);
			fprintf(stderr, YELLOW " -> " RESET);
			lst = lst->next;
		}
		cprintf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

// Prints from lst to end
void	ft_lstprint_betw(t_list *lst, t_list *end)
{
	int	count;

	count = 0;
	if (lst == NULL || end == NULL)
		return ;
	if (DEBUG)
	{
		cprintf(RED, "ll segment: " RESET);
		while (lst != end->next)
		{
			count++;
			fprintf(stderr, YELLOW "%s -> " RESET, (const char *)lst->content);
			lst = lst->next;
		}
		cprintf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}
