/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_int.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:08 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:09 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_INT_H
# define EXECUTE_INT_H

# include "execute.h"

# define _MOD_ "Exec"
# define NO_APND 0

/* Forwards */

extern size_t	write_heredoc(int fd, t_lex *l);
extern size_t	read_heredoc(int fd, t_lex *l);

int				handle_exit(t_state *s, int code);

#endif