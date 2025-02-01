#include "command_int.h"

	// I think fork?
	// loop, get exit status, decide to proceed
	// (recurse)
	// return last exit status
int cmd_exec_log(t_state *s, t_ast_node *a)
{
	debug_print("\t### cmd_exec_log ###\n");
	(void)s;
	(void)a;
	return (0);
}
