/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_int.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:27:57 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:27:58 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_INT_H
# define HASHTABLE_INT_H

# include "hashtable.h"

struct					s_ht_entry
{
	struct s_ht_entry	*next;
	char				*name;
	void				*data;
};

struct					s_ht
{
	struct s_ht_entry	*buckets[HASHSIZE];
};

unsigned int			hash(const char *s);

#endif
