#include "state_int.h"

// Sets error code
void	set_error(t_state *state, int code)
{
	state->error_code = code;
}
