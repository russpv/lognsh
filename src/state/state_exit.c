#include "state.h"

/* Exits without leaks. */
void	exit_clean(t_mem_node *head, int exit_status, const char *caller,
		const char *errmsg)
{
	if (errmsg)
		print_custom_err_err(SHELL_NAME, caller, errmsg);
	mem_free_all(head);
	if (exit_status)
		exit(exit_status);
	exit(ENOMEM);
}
