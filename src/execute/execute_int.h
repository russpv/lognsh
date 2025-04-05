/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_int.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:49:21 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 21:05:25 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_INT_H
# define EXECUTE_INT_H

# include "execute.h"
# define _MOD_ "Exec"

/* Forwards */

extern size_t	write_heredoc(int fd, t_lex *l);
extern size_t	read_heredoc(int fd, t_lex *l);

int				handle_exit(t_state *s, int code);

#endif
