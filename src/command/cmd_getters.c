#include "command_int.h"

char *c_getfullpath(t_cmd *c)
{
	return (c->fullpath);
}

char **c_getargv(t_cmd *c)
{
	return (c->argv);
}

t_stack *c_getctxt(t_cmd *c)
{
	return (c->st);
}

t_ast_node *c_getnode(t_cmd *c)
{
	return (c->curr_node);
}
