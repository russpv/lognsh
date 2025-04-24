
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