/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:32:35 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/27 17:17:26 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Low-level implementation of forking, piping, redirects
#ifndef EXECUTE_H
#define EXECUTE_H

#include "../builtins/builtins.h"
#include "../parse/parse.h"

// Fn pointer type for built-in commands
typedef void (*t_builtin_fn)(char **args);

t_builtin_fn    get_builtin(char *command); //map cmd to built-in fn
void    execute_command(t_ast_node *node); //exec a cmd
char    **list_to_array(t_list *args, int argc); //convert linked lst of args to array
//int     execute_system_command(char **args;) TODO

#endif
