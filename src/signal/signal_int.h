/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_int.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:58:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/04 00:01:29 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_INT_H
# define SIGNAL_INT_H

# include "signal.h"

# define _MOD_ "Signal"

typedef struct s_signal_manager
{
	struct sigaction sa_int;  // SIGINT (Ctrl-C)
	struct sigaction sa_quit; // SIGQUIT (Ctrl-\)
}		t_signal_mgr;

void	sigint_handler(int signo);
void	sigquit_handler(int signo);
#endif
