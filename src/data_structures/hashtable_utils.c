
#include "hashtable_int.h"

void	*ht_get_payload(struct s_ht_entry *e)
{
	return (e->data);
}

/* Optionally copies the data before assigning it. */
static int	_install_data(struct s_ht_entry *np, void *data, \
		void *(*cpy)(void *))
{
	if (NULL != data)
	{
		if (cpy)
			np->data = cpy(data);
		else
			np->data = data;
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
struct s_ht_entry	*ht_install(t_ht ht, char *name, void *data,
		void *(*cpy_data)(void *))
{
	struct s_ht_entry *np;
	unsigned int hashval;

	if (NULL == name)
		return (NULL);
	np = ht_lookup(ht, name);
	if (NULL == np)
	{
		np = (struct s_ht_entry *)malloc(sizeof(*np));
		if (NULL == np)
			return (NULL);
		np->name = ft_strdup(name);
		if (NULL == np->name)
		{
			free(np);
			return (NULL);
		}
		hashval = hash(name);
		np->next = ht->buckets[hashval];
		ht->buckets[hashval] = np;
		if (-1 == _install_data(np, data, cpy_data))
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
