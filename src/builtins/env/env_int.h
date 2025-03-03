/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 21:06:33 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 19:35:32 by dayeo            ###   ########.fr       */
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
