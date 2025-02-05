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

int	bi_echo(t_state *s, char **args, int argc);
int	bi_pwd(t_state *s, char **args, int argc);
int	bi_exit(t_state *s, char **args, int argc);
int    bi_env(t_state *s, char **argv, int argc);
int	bi_exec(t_state *s, char **args, int argc); //RKP so shell can be relaunched without exit



#endif
