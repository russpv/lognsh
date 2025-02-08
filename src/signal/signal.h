/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:02:00 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/08 01:48:00 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include "../globals/globals.h"
# include "../state/error.h"
# include "../state/state.h"
# include <signal.h>

// register signal handlers
void    set_signal_handlers(void);

// un-register signal handlers back to defaults
void	reset_signal_handlers(void);

int	    handle_last_signal(void);
// turn this into an ADT 
/*
struct s_signal_manager;
{

}

typedef struct s_signal_manager *t_signal_mgr;
*/
#endif
