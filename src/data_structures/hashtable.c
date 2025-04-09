#include "hashtable_int.h"

/* This defines an array of linked lists
 * for indexing
 * Stores multiple entries at an index
 * so no data is lost
 * Inspired by K&R Ch 6.6
 */

t_ht	ht_create(t_mem_mgr *m)
{
	int		i;
	t_ht	hasht;

	i = -1;
	hasht = m->f(&m->list, sizeof(struct s_ht));
	if (hasht)
	{
		while (++i < HASHSIZE)
			hasht->buckets[i] = NULL;
	}
	return (hasht);
}

unsigned int	hash(const char *s)
{
	unsigned int	hashval;

	hashval = 0;
	if (NULL == s)
		return (0);
	while (*s)
		hashval = (unsigned char)*s++ + 31 * hashval;
	return (hashval % HASHSIZE);
}

/* Returns node of unique name */
struct s_ht_entry	*ht_lookup(t_ht ht, char *s)
{
	struct s_ht_entry	*np;

	np = ht->buckets[hash(s)];
	while (NULL != np)
	{
		if (0 == ft_strncmp(s, np->name, (size_t)(-1)))
			return (np);
		np = np->next;
	}
	return (NULL);
}

int	ht_destroy(t_mem_mgr *m, t_ht hasht, void (*del)(t_mem_node *, void *))
{
	struct s_ht_entry	*np;
	struct s_ht_entry	*tmp;
	size_t				i;

	i = (size_t)-1;
	while (++i < HASHSIZE)
	{
		np = hasht->buckets[i];
		if (NULL == np)
			continue ;
		while (NULL != np)
		{
			tmp = np->next;
			if (np->name)
				m->dealloc(&m->list, np->name);
			if (np->data)
				del(&m->list, np->data);
			m->dealloc(&m->list, np);
			np = tmp;
		}
	}
	m->dealloc(&m->list, hasht);
	return (0);
}

void	ht_init_utils(t_ht_utils *u, t_ht h, t_cpy_fn f)
{
	u->cpy_data = f;
	u->ht = h;
}
