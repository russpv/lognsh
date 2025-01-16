#ifndef LLIST_H
# define LLIST_H

/* LINKED LIST */

typedef	struct s_llnode	*t_ll;

t_ll	ll_create();
//ll_clear(t_ll ll); //destroy all nodes except head
ll_destroy(t_ll ll);

ll_insert_head(t_ll ll, void *data);
ll_insert_tail(t_ll ll, void *data);
//ll_insert_idx(t_ll ll, void *data, size_t pos);
ll_remove_head(t_ll ll);
//ll_remove_tail(t_ll ll);
//ll_remove_idx(t_ll ll, size_t pos);

/* Getters */
ll_get_firstn(t_ll ll);
ll_get_lastn(t_ll ll);
ll_get_value();
//ll_get_index();
//ll_get_size(t_ll ll);
//ll_is_empty(t_ll ll);

/* Setters */
//ll_update(t_ll ll, size_t pos);
//ll_iter_do(t_ll ll, (*action)(void *));

#endif
