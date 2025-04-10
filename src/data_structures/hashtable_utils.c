/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:29:53 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:29:54 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable_int.h"

void	*ht_get_payload(struct s_ht_entry *e)
{
	if (e)
		return (e->data);
	return (NULL);
}

/* Optionally copies the data before assigning it. */
static int	_install_data(t_mem_mgr *m, struct s_ht_entry **np, void *data,
		void *(*cpy)(t_mem_node *, void *))
{
	if (NULL != data)
	{
		if (cpy)
			(*np)->data = cpy(&m->list, data);
		else
			(*np)->data = data;
		if (NULL == (*np)->data)
			return (-1);
	}
	else
		(*np)->data = NULL;
	return (0);
}

static int	_init_node(t_mem_mgr *m, t_ht ht, struct s_ht_entry **np,
		char *name)
{
	unsigned int	hashval;

	*np = (struct s_ht_entry *)m->f(&m->list, sizeof(struct s_ht_entry));
	if (NULL == np)
		return (-1);
	(*np)->name = ft_strdup_tmp(m, name);
	if (NULL == (*np)->name)
	{
		m->dealloc(&m->list, np);
		return (-1);
	}
	hashval = hash(name);
	(*np)->next = ht->buckets[hashval];
	ht->buckets[hashval] = *np;
	return (0);
}

/* Makes new entry the head for the hash bucket
 * Returns NULL if name is already present
 */
struct s_ht_entry	*ht_install(t_mem_mgr *m, t_ht_utils *u, char *name,
		void *data)
{
	struct s_ht_entry	*np;

	if (NULL == name)
		return (NULL);
	np = ht_lookup(u->ht, name);
	if (NULL == np)
	{
		if (-1 == _init_node(m, u->ht, &np, name))
			return (NULL);
		if (-1 == _install_data(m, &np, data, u->cpy_data))
		{
			m->dealloc(&m->list, np->name);
			m->dealloc(&m->list, np);
			return (NULL);
		}
	}
	else
		return (NULL);
	return (np);
}
