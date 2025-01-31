/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_int.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:57:34 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:10:23 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BI_INT_H
# define BI_INT_H

#include "bi.h"

#include "../../include/libft.h"
#include "../../debug.h"
#include <unistd.h>
#include <stdio.h>



int	bi_echo(char **args);
int	bi_pwd(char **args);
//void	bi_env(char **args); //Note: compile errors
//void	bi_exit(char **args);
//void    bi_cd(char **args);


#endif