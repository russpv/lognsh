/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:35:15 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 15:35:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.h"

void	sigint_killchild(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
	}
}

// Unblocks readline() 
void	sigint_killrl(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
		close(STDIN_FILENO);
	}
}

// SIGQUIT (Ctrl-\) handler
void	sigquit_handler(int signo)
{
	if (signo == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
			return ;
		g_last_signal = SIGQUIT;
	}
}
