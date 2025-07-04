#include "llist_int.h"


/* -------------------------------------------------------------------------
 * Function Group: linked list sort comparison logic
 * Description   : 
 * ------------------------------------------------------------------------- */

void	get_root(char *buf, void *s)
{
	char	*str;
	size_t	buf_idx;

	str = (char *)s;
	buf_idx = 0;
	while (*str && buf_idx < MAX_BUF)
	{
		if (ft_isalnum(*str))
			buf[buf_idx++] = *str;
		str++;
	}
}

static const char	*_strip_prespecials(void *s)
{
	char	*str;

	str = (char *)s;
	while (*str && !ft_isalnum(*str))
		str++;
	return (str);
}

// Check if both strings contain alphanumeric characters
static int	_check_both_alphanumeric(const char *str1, const char *str2,
		const char *root1, const char *root2)
{
	if (str_hasalnum(str1) && str_hasalnum(str2))
	{
		if ('$' == *str1 && '$' != *str2)
			return (ALESSTHANB);
		if ('$' != *str1 && '$' == *str2)
			return (AMORETHANB);
		if ('$' == *str1 && '$' == *str2)
			return (ft_strcmp_casefold(str1, str2));
		if (0 == ft_strcmp_low(root1, root2) && *root1 && *root2)
		{
			if (0 != ft_strcmp_casefold(root1, root2))
				return (ft_strcmp_casefold(root1, root2));
			while (*str1 && *str1 == *str2)
			{
				str1++;
				str2++;
			}
			if (*str1 != *str2)
				return (*str1 - *str2);
		}
		return (ft_strcmp_casefold(root1, root2));
	}
	return (0);
}

// Handle the case where at least one string has no alphanumeric characters
static int	_check_one_alphanumeric(const char *str1, const char *str2,
		const char *body1, const char *body2)
{
	if (!str_hasalnum(str1) && str_hasalnum(str2))
	{
		if ('$' == *str1 && '$' != *str2)
			return (ALESSTHANB);
		if ('$' != *str1 && '$' == *str2)
			return (ALESSTHANB);
		if (str_isalnum(str2))
			return (ALESSTHANB);
		return (ft_strcmp_casefold(str1, body2));
	}
	else if (str_hasalnum(str1) && !str_hasalnum(str2))
	{
		if ('$' == *str1 && '$' != *str2)
			return (AMORETHANB);
		if ('$' != *str1 && '$' == *str2)
			return (AMORETHANB);
		if (str_isalnum(str1))
			return (AMORETHANB);
		return (ft_strcmp_casefold(body1, str2));
	}
	return (0);
}

// Both strings are special chars, handle '$' prefixed strings
static int	_check_both_specials(const char *str1, const char *str2)
{
	if ('$' == *str1 && '$' != *str2)
		return (AMORETHANB);
	if ('$' != *str1 && '$' == *str2)
		return (ALESSTHANB);
	return (ft_strcmp(str1, str2));
}

// Ranks special chars ahead of alphanums; and $-prefixed strings
// higher than varname strings
// +haha and (haha appear ahead of haha but after foo
int	compare(t_list *beg, t_list *end)
{
	const char	*body1 = _strip_prespecials(beg->content);
	const char	*body2 = _strip_prespecials(end->content);
	char		root1[MAX_BUF];
	char		root2[MAX_BUF];
	int			res;

	ft_memset(root1, 0, MAX_BUF);
	ft_memset(root2, 0, MAX_BUF);
	get_root(root1, beg->content);
	get_root(root2, end->content);
	res = _check_both_alphanumeric(beg->content, end->content, root1, root2);
	if (res == 0)
		res = _check_one_alphanumeric(beg->content, end->content, body1, body2);
	if (res == 0)
		res = _check_both_specials(beg->content, end->content);
	if (res != 0)
	{
		if (res > 0)
			return (AMORETHANB);
		else
			return (ALESSTHANB);
	}
	return (0);
}


/* -------------------------------------------------------------------------
 * Function Group: linked list mergesort logic
 * Description   : 
 * ------------------------------------------------------------------------- */

/* Adds the remaining nodes after one of the
 * left or right sub-lists is exhausted.
 */
static void	_add_remainder(struct s_merge *m)
{
	if (m->l != m->mid->next)
	{
		*m->tail = m->l;
		(*m->tail)->prev = m->tmp;
	}
	else if (m->r != m->tmp2)
	{
		*m->tail = m->r;
		(*m->tail)->prev = m->tmp;
	}
	print_addremainder_post(m);
}

/* Swaps the current tail node for the correct
 * one across left and right sub-lists. Reassigns
 * prev ptr to where tail was last (tmp).
 */
static void	_do_swap(t_list **lst, struct s_merge *m)
{
	if (ALESSTHANB == compare(m->l, m->r))
	{
		*m->tail = m->l;
		m->l = m->l->next;
	}
	else if (AMORETHANB == compare(m->l, m->r))
	{
		*m->tail = m->r;
		m->r = m->r->next;
	}
	if ((*m->tail) != NULL && m->tmp != NULL)
	{
		print_prev_pre(m);
		(*m->tail)->prev = m->tmp;
		print_prev_post(m);
	}
	print_swap_post(lst, m);
}

/* Advances to the end of the added remainder
 * and ensures next ptr is set to reconnect
 * with rest of list beyond this segment
 */
static void	_adv_and_reconnect(struct s_merge *m)
{
	while (++m->i < m->nodec)
		m->tail = &((*m->tail)->next);
	print_tailadvance_post(m);
	if (*m->tail)
		(*m->tail)->next = m->tmp2;
	print_tailnext_post(m);
}

/* Ensures the true head and true tail are terminated NULL
 * in case any swap occurred.
 */
void	_terminate_ends(t_list **lst, struct s_merge *m)
{
	if ((*m->end)->next == NULL && *m->tail == *m->end)
		(*m->tail)->next = NULL;
	(*lst)->prev = NULL;
	print_diagnostics_end(lst, m);
}

/* MERGESORT
 * Google it.
 * Note: Critical that the comparator function does not return ties or the tail
 * advances to whichever node->next.
 * Watch for:
 * - swapped node can be the head ptr with no prev!
 * - swapped node can be the end ptr with no next!
 * Critical that list segment boundary ptrs are updated for subsequent
 * recursive calls as stack unwinds, or segments may be skipped.
 */

void	merge(t_list **lst, t_list **beg, t_list *mid, t_list **end)
{
	struct s_merge	m;

	init_merge(&m, beg, end, mid);
	print_diagnostics_input(lst, &m);
	if (ALESSTHANB == compare(mid, mid->next))
		return (dvprint("%s: already sorted.\n", __FUNCTION__));
	while ((NULL != m.l && NULL != m.r) && (mid->next != m.l
			&& (*end)->next != m.r) && (++m.i <= m.nodec))
	{
		_do_swap(lst, &m);
		m.tmp = *m.tail;
		m.tail = &((*m.tail)->next);
		dprint_tail_info(&m);
	}
	_add_remainder(&m);
	_adv_and_reconnect(&m);
	if (*m.tail)
		(*end) = (*m.tail);
	_terminate_ends(lst, &m);
	return ;
}
