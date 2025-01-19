#include "libft.h"

/* LSTNEW
** Returns new linked list head ptr with one node
*/

t_list	*ft_lstnew(void *content)
{
	t_list	*ll;

	ll = malloc(sizeof(t_list));
	if (!ll)
		return (NULL);
	ll->next = NULL;
	ll->content = content;
	return (ll);
}
