/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:02:00 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/27 17:40:49 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include "../globals/globals.h"
# include "../state/error.h"
# include "../utils/debug.h"
# include <readline/readline.h>

# define SIGINT_BEFORE_FORK -130
# define SIGEOF_AT_INPUT 300
# define INT_KCHILD 0
# define INT_DFL 1
# define INT_KRL 2


/* Global signal variable is checked periodically
 * through execution chain, and triggers appropriate
 * action and handler. This received signal is reset
 * in the main loop. When forking children, handling
 * in the parent is suspended (ignores sigs).
 */

// register signal handlers
void	sig_set_handlers(int option);

// un-register signal handlers back to defaults
void	sig_reset_handlers(void);

void	sig_ignore(void);

void	clear_current_line(void);

#endif
