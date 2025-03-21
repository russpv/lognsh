#include "state_int.h"

// Sets error code
void	set_error(t_state *state, int code)
{
	state->error_code = code;
}

int	set_exit_status(t_state *state, int value)
{
	state->current_exit_code = value;
	return (0);
}
