/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:55:34 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:55:35 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"

/* Exits without leaks. */
void	exit_clean(t_mem_node *head, int exit_status, const char *caller,
		const char *errmsg)
{
	if (errmsg)
		print_custom_err(caller, errmsg);
	mem_free_all(head);
	if (exit_status)
		exit(exit_status);
	exit(ENOMEM);
}
