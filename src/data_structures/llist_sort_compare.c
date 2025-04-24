
#include "llist_int.h"

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
