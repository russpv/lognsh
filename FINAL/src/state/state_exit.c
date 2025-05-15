/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:45 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:46 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"

/* Exits without leaks. */
void	exit_clean(t_mem_node *head, int exit_status, const char *caller,
		const char *errmsg)
{
	if (errmsg)
		print_custom_err_err(SHELL_NAME, caller, errmsg);
	mem_free_all(head);
	if (exit_status != ENOMEM)
		exit(exit_status);
	exit(ENOMEM);
}
