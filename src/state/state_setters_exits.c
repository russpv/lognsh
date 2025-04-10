/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_exits.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:59 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:00 by rpeavey          ###   ########.fr       */
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
