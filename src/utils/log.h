/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:58:38 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:58:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# include "../../include/ansi_colors.h"
# include <assert.h>
# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

# ifdef LOGMODE
#  define LOGGING 1
# else
#  define LOGGING 0
# endif

/* Forwards */
typedef struct s_cmd	t_cmd;
typedef struct s_node	t_ast_node;

void					log_command_info(t_cmd *c, t_ast_node *a);
void					log_print(const char *s, ...);
void					colored_printf(const char *color, const char *format,
							...);
void					err(const char *s);

#endif