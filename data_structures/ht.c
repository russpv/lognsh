#include "hashtable.h"

/* This defines an array of linked lists
 * for indexing
 * Stores multiple entries at an index
 * so no data is lost
 * Inspired by K&R Ch 6.6
 */

//static struct s_ht_entry	*g_hasht[HASHSIZE];

struct s_ht_entry {
	struct s_ht_entry *next;
	char *name;
	void *data;
};

struct s_ht {
	struct s_ht_entry	*buckets[HASHSIZE];
};

t_ht	ht_create()
{
	int i;

	i = -1;
	t_ht hasht = malloc(sizeof(struct s_ht));
	if (hasht)
	{
		while (++i < HASHSIZE)
			hasht->buckets[i] = NULL;
	}
	return (hasht);
}

static unsigned int	hash(char *s)
{
	unsigned int	hashval;

	hashval = 0;
	while (*s)
		hashval = *s++ + 31 * hashval;
	return (hashval % HASHSIZE);
}

/* Returns node of unique name */
struct s_ht_entry	*lookup(t_ht ht, char *s)
{
	struct s_ht_entry	*np;

	np = ht->buckets[hash(s)];
	while (NULL != np)
	{
		if (0 == ft_strncmp(s, np->name, -1))
			return (np);
		np = np->next;
	}
	return (NULL);
}

static int	_install_data(struct s_ht_entry *np, void *data, void *(*cpy)(void *))
{
	if (NULL != data)
	{
		np->data = cpy(data);
		if (NULL == np->data)
			return (-1);
	}
	else
		np->data = NULL;
	return (0);
}

/* Makes new entry the head for the hash bucket 
 * Returns NULL if name is already present
 */
struct s_ht_entry	*install(t_ht ht, char *name, char *data, void *(*cpy)(void *))
{
	struct s_ht_entry	*np;
	unsigned int	hashval;

	if (NULL == name)
		return (NULL);
	np = lookup(ht, name);
	if (NULL == np)
	{
		np = (struct s_ht_entry *)malloc(sizeof(*np));
		if (NULL == np)
			return (NULL);
		np->name = ft_strdup(name);
		if (NULL == np->name)
			return (NULL);
		hashval = hash(name);
		np->next = ht->buckets[hashval];
		ht->buckets[hashval] = np;
		if (-1 == _install_data(np, data, cpy))
			return (NULL);
	}
	else
		return (NULL);
	return (np);
}

int	destroy_hasht(t_ht hasht, void (*del)(void *))
{
	struct s_ht_entry	*np;
	struct s_ht_entry	*tmp;
	size_t			i;

	i = -1;
	while (++i < HASHSIZE)
	{
		np = hasht->buckets[i];
		if (NULL == np)
			continue ;
		while (NULL != np)
		{
			tmp = np->next;
			free(np->name);
			if (NULL != np->data)
				del(np->data);
			free(np);
			np = tmp;
		}
	}
	return (0);
}

void *ht_get_payload(struct s_ht_entry *e)
{
	return (e->data);
};

