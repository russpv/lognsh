#include "state_int.h"

void	set_error(t_state *state, int code)
{
	state->error_code = code;
}
