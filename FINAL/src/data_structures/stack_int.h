/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:31:20 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 19:45:26 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_INT_H
# define STACK_INT_H

# include "stack.h"
# include "../utils/debug.h"

# define MAX_ST_DEPTH 10
# define DBGMSG_STPUSH_OVERFLOW "ERROR Stack overflow!\n"
# define DBGMSG_STPOP_UNDERFL "ERROR Stack underflow!\n"

struct						s_int_stack
{
	int						st[MAX_ST_DEPTH];
	int						depth;
};

typedef struct s_int_stack	t_int_stack;

struct						s_ptr_stack
{
	void					*st[MAX_ST_DEPTH];
	int						depth;
};

typedef struct s_ptr_stack	t_ptr_stack;

#endif