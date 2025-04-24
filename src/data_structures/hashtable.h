
#ifndef HASHTABLE_H
# define HASHTABLE_H

# include "../../include/libft.h"
# include "i_llist.h"
# include <assert.h>
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define HASHSIZE 101

/* Forward declaration */
struct				s_ht_entry;
struct				s_ht;

typedef struct s_ht	*t_ht;

typedef void		*(*t_cpy_fn)(t_mem_node *, void *);

typedef struct s_ht_utils
{
	t_cpy_fn		cpy_data;
	t_ht			ht;
}					t_ht_utils;

t_ht				ht_create(t_mem_mgr *m);
int					ht_destroy(t_mem_mgr *m, t_ht hasht,
						void (*del)(t_mem_node *, void *));

void				ht_init_utils(t_ht_utils *u, t_ht h, t_cpy_fn f);

void				*ht_get_payload(struct s_ht_entry *e);
struct s_ht_entry	*ht_lookup(t_ht ht, char *s);
struct s_ht_entry	*ht_install(t_mem_mgr *m, t_ht_utils *u, char *name,
						void *data);

#endif
