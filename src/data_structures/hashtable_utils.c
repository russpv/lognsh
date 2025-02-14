#include "hashtable_int.h"

void	*ht_get_payload(struct s_ht_entry *e)
{
	return (e->data);
}

/* Optionally copies the data before assigning it. */
static int	_install_data(struct s_ht_entry **np, void *data,\
		void *(*cpy)(void *))
{
	if (NULL != data)
	{
		if (cpy)
			(*np)->data = cpy(data);
		else
			(*np)->data = data;
		if (NULL == (*np)->data)
			return (-1);
	}
	else
		(*np)->data = NULL;
	return (0);
}

static int	_init_node(t_ht ht, struct s_ht_entry **np, char *name)
{
	unsigned int	hashval;

	*np = (struct s_ht_entry *)malloc(sizeof(struct s_ht_entry));
	if (NULL == np)
		return (-1);
	(*np)->name = ft_strdup(name);
	if (NULL == (*np)->name)
	{
		free(np);
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
struct s_ht_entry	*ht_install(t_ht ht, char *name, void *data,\
		void *(*cpy_data)(void *))
{
	struct s_ht_entry	*np;

	if (NULL == name)
		return (NULL);
	np = ht_lookup(ht, name);
	if (NULL == np)
	{
		if (-1 == _init_node(ht, &np, name))
			return (NULL);
		if (-1 == _install_data(&np, data, cpy_data))
		{
			free(np->name);
			free(np);
			return (NULL);
		}
	}
	else
		return (NULL);
	return (np);
}
