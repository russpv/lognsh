/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:20:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/13 14:18:24 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/signal/signal.h"
# include "../src/state/state.h" 
# include "../src/command/command.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <errno.h>
# include <ctype.h>

/* For compatibility */
# define MYTEST 0

#endif
