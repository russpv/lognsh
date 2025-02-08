/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:02:00 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/04 00:18:15 by dayeo            ###   ########.fr       */
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

// RKP: this module doesn't manage any state, or need to, so struct is not really needed

#endif
