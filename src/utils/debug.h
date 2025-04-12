/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:27 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/13 01:58:20 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "../../include/ansi_colors.h"
# include <assert.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>

# ifdef DEBUGMODE
#  define DEBUG 1
# else
#  define DEBUG 0
# endif

# ifdef DEBUGVMODE
#  define DEBUGVERBOSE 1
# else
#  define DEBUGVERBOSE 0
# endif

# define DEBUG_BUF_SIZE 80

extern size_t			ft_strnlen(const char *s, size_t maxlen);

typedef struct s_list	t_list;

void					dprint(const char *format, ...);
void					dvprint(const char *format, ...);
void					lgprint(const char *s, ...);
void					dprintdiv(const char *format, ...);

void					err(const char *s);
void					cprintf(const char *color, const char *format, ...);
void					debug_detect_cycle(t_list *head);

void					print_array(char *arr[]);
bool					test_prev_integrity(t_list *lst);

#endif
