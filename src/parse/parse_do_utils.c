#include "parse_int.h"

int	p_is_arg_null(void *c)
{
	t_arg_data	*content;

	content = (t_arg_data *)c;
	if (NULL == c)
		return (-1);
	if (true == content->is_grouparg)
		return (0);
	if (NULL == content->raw)
		return (1);
	return (0);
}

/* Add special $? codes here.
 * buf will start with char after any '$'.
 * Returns the negated length of the expansion found (minus $).
 * Or error codes.
 * Places the expanded heap string in *value
 */
int	check_special_expansions(t_state *s, const char *buf, char **value)
{
	const int	*status = get_status(s);

	if (!buf || !value)
		return (ERR_ARGS);
	dprint(PDMSG_IN, _MOD_, __FUNCTION__, buf);
	if (ft_strcmp(buf, "?") == 0)
	{
		if (status == NULL)
			*value = ft_itoa_mem(&get_mem(s)->list, myalloc, 0);
		else
			*value = ft_itoa_mem(&get_mem(s)->list, myalloc, *status);
		if (*value == NULL)
			return (ERR_MEM);
		dprint(PDMSG_OUT, _MOD_, __FUNCTION__, *value);
		return (-1);
	}
	return (0);
}
