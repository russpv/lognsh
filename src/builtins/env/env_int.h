/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 21:06:33 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/12 18:57:46 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INT_H
# define ENV_INT_H

# include "env.h"

// Env variable linked list node
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

#endif
