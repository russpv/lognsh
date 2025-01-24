// Probably need error handler with value map to messages

#include "error.h"

// Function to return NULL and set the error code in the state
void	*null_and_stat(t_state *s, int error_code)
{
	if (s)
	{
		set_error(s, error_code);
	}
	return (NULL);
}