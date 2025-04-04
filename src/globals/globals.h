/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:44:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 14:44:49 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "minish"
//# define DFL_PROMPT GREEN SHELL_NAME "> " RESET
# define DFL_PROMPT "\001\033[32m\002minish> \001\033[0m\002"
# define HDOC_PROMPT "> "
# define INPUT_BUF_SZ 1024
# define IFS " \t\n"

extern volatile sig_atomic_t	g_last_signal;

#endif
