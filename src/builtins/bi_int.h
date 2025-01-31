/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_int.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:57:34 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/01 01:02:17 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BI_INT_H
# define BI_INT_H

#include "bi.h"

int	bi_echo(t_state *s, char **args);
int	bi_pwd(t_state *s, char **args);
int	bi_exit(t_state *s, char **args);

//void	bi_env(char **args); //Note: compile errors
//void    bi_cd(char **args);


#endif