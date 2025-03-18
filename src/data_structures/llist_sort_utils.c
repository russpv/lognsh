#include "llist_int.h"


bool	str_isalnum(const char *s)
{
	if (!s)
		return false;
	while (*s)
	{
		if (!ft_isalnum(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	str_hasalnum(const char *s)
{
	if (!s)
		return false;
	while (*s)
	{
		if (ft_isalnum(*s))
			return (true);
		s++;
	}
	return (false);
}


void	init_merge(struct s_merge *m, t_list **beg, t_list **end, t_list *mid)
{
	m->i = 0;
	m->tail = beg;
	m->tmp = NULL;
    m->l = *beg;
    m->r = mid->next;
	m->tmp2 = &(*((*end)->next)); // need this to reconnect segment with rest of list
	m->nodec = ft_lstsize_betw(*beg, *end);
	m->mid = mid;
	m->beg = beg;
	m->end = end;
}


/*
// Ranks special chars ahead of alphanums; and $-prefixed strings higher than varname strings
// +haha and (haha appear ahead of haha but after foo
static int    _compare(t_list *beg, t_list *end)
{
	const char *str1 = beg->content;
	const char *str2 = end->content;
	const char *body1 = _strip_prespecials(beg->content);
	const char *body2 = _strip_prespecials(end->content);
	char root1[MAX_BUF];
	char root2[MAX_BUF];
	ft_memset(root1, 0, MAX_BUF);
	ft_memset(root2, 0, MAX_BUF);
	_get_root(root1, beg->content);
	_get_root(root2, end->content);

	int res;

	if (_str_hasalnum(str1) && _str_hasalnum(str2)) //at least one with special pre, alpha bodies
	{ 
		if ('$' == *str1 && '$' != *str2) // '$PATH' vs '0'
			return ALESSTHANB;
		if ('$' != *str1 && '$' == *str2) //lower
			return AMORETHANB;
		if ('$' == *str1 && '$' == *str2) // '$PATH' vs $aaa
			res = ft_strcmp_casefold(str1, str2);
		else if (0 == ft_strcmp_low(root1, root2) && *root1 && *root2) // if same roots, compare prefixes
		{
			if (0 != ft_strcmp_casefold(root1, root2))
			{
				fprintf(stderr, "diff roots: %s vs %s\n", str1, str2);
				res = ft_strcmp_casefold(root1, root2);
			}
			else
			{
				while (*str1 && *str1 == *str2) 
				{
					str1++;
					str2++;
				}
				if (*str1 != *str2)
				{
					res = *str1 - *str2;
					return res > 0 ? AMORETHANB  : ALESSTHANB;
				}
			}
		}
		else // different roots
		{
			//fprintf(stderr, "diff roots: %s vs %s\n", str1, str2);
			res = ft_strcmp_casefold(root1, root2);
		}
		if (res != 0)
			return res > 0 ? AMORETHANB : ALESSTHANB;
	}
	else // at least one without any alphas
	{
		if (!_str_hasalnum(str1) && _str_hasalnum(str2)) // str1 full special
		{
			if ('$' == *str1 && '$' == *str2) // '$' vs '$PATH"
				res = ft_strcmp_casefold(str1, str2);
			else if ('$' == *str1 && '$' != *str2) //'$' vs <haha, 0
				return ALESSTHANB;
			else if ('$' != *str1 && '$' == *str2) //'?', vs $PATH
				return ALESSTHANB;
			else if (_str_isalnum(str2))// neither have '$' ? vs 0 <haha
				return ALESSTHANB;
			res = ft_strcmp_casefold(str1, body2);
		}
		else if (_str_hasalnum(str1) && !_str_hasalnum(str2)) //str2 full special
		{
			if ('$' == *str1 && '$' == *str2) // '$PATH' vs '$'
				res = ft_strcmp_casefold(str1, str2);
			else if ('$' == *str1 && '$' != *str2) // $PATH vs '?'
				return AMORETHANB;
			else if ('$' != *str1 && '$' == *str2) //'<haha' vs '$'
				return AMORETHANB;
			else if (_str_isalnum(str1))
				return AMORETHANB;
			res = ft_strcmp_casefold(body1, str2);
		}
		else if (!_str_hasalnum(str1) && !_str_hasalnum(str2)) // both full special
		{
			if ('$' == *str1 && '$' != *str2) // '$' vs '*'
				return AMORETHANB;
			if ('$' != *str1 && '$' == *str2) // '*' vs '$'
				return ALESSTHANB;
			res = ft_strcmp_casefold(str1, str2);
		}	
		if (res != 0)
			return res > 0 ? AMORETHANB : ALESSTHANB;
	}
	return (0);
}
*/