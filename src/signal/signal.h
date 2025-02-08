/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:02:00 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 03:03:13 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include "../globals/globals.h"
# include "../state/error.h"
# include "../state/state.h"
# include <signal.h>

# define SIGINT_BEFORE_FORK -130

// register signal handlers
void    set_signal_handlers(void);

// un-register signal handlers back to defaults
void	reset_signal_handlers(void);

int	    handle_last_signal(void);

#endif
