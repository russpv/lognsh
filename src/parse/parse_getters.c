#include "parse_int.h"

char	*p_get_fn(const t_redir_data *red)
{
	if (!red)
		return (NULL);
	return (red->filename);
}
