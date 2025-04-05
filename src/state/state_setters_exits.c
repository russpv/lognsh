/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_exits.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:57:04 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:57:05 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

// Sets error code
void	set_error(t_state *state, int code)
{
	state->error_code = code;
}

// Sets exit code
void	set_exit_status(t_state *state, int value)
{
	if (!state)
		return ;
	state->current_exit_code = value;
}
