#include "hashtable_int.h"

/* This defines an array of linked lists
 * for indexing
 * Stores multiple entries at an index
 * so no data is lost
 * Inspired by K&R Ch 6.6
 */

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
	(*np)->name = ft_strdup_mem(m, name);
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
