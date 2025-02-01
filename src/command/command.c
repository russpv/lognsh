#include "command_int.h"

void	destroy_cmd(void *c)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)c;
	st_destroy(cmd->st);
	free(cmd);
}

t_cmd	*init_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;

	c = malloc(sizeof(struct s_cmd));
	c->st = st_create();
	c->argv = NULL;
	c->fullpath = NULL;
	c->curr_node = a;
	c->saved_stderr = -1;
	c->saved_stdin = -1;
	c->saved_stdout = -1;
	set_command(s, c);
	register_command_destroy(s, destroy_cmd);
	return (c);
}

/* Can accept null args and cmd
 * If no cmd, copies first arg as cmd
 * Does not need to handle no-command situation
 */
char	**c_argstoargv(char **args, char *cmd, int argc)
{
	char	**argv;
	int		i;

	i = -1;
	argv = malloc(sizeof(char *) * (argc + 2));
	if (argv)
	{
		while (++i <= argc)
		{
			if (cmd && i == 0)
				argv[i] = ft_strdup(cmd);
			else if (!cmd && i == 0 && args && argc > 0)
				argv[i] = ft_strdup(args[i]);
			else if (cmd && i != 0 && args && argc > 0)
				argv[i] = ft_strdup(args[i - 1]);
			else if (!cmd && i != 0 && args && argc > 0)
				argv[i] = ft_strdup(args[i]);
			if (!argv[i])
			{
				ft_freearr((void **)argv, -1);
				return (NULL);
			}
		}
		argv[i] = NULL;
	}
	return (argv);
}

int cmd_exec_log(t_state *s, t_ast_node *a)
{
	debug_print("\t### cmd_exec_log ###\n");
	(void)s;
	(void)a;
	return (0);
	// I think fork?
	// loop, get exit status, decide to proceed
	// (recurse)
	// return last exit status
}

	//set up pipes
	//fork, cmd_execute_full(next node)
int cmd_exec_pipe(t_state *s, t_ast_node *a)
{
	debug_print("\t### cmd_exec_pipe ###\n");
	//create pipes
	(void)s;
	(void)a;
	return (0);
}

// Even though it stores a t_list, it can 
// only be one command
int cmd_exec_proc(t_state *s, t_ast_node *a)
{
	debug_print("\t### cmd_exec_proc ###\n");
	const int cmdc = p_get_proc_cmdc(a);
	const t_list *cmd = p_get_proc_cmds(a);
	t_ast_node *node = NULL;
	int i;

	i = -1;
	if (0 == cmdc || NULL == cmd)
		return (0);
	while (++i < cmdc)
	{
		//push onto stack?
		node = cmd->content;
		exec_fork_func(s, node, cmd_execute_full);
		cmd = cmd->next;
	}
	return (0);
}

int	cmd_execute_full(t_state *s, t_ast_node *a)
{
	/*
	if (is_at_end(a)) // may be unnecessary
	{
		debug_print("reached end of ast\n");
		return (NULL);
	}*/
	debug_print("######## cmd_execute_full ########\n");
	if (AST_NODE_PROC == p_get_type(a))
		return (cmd_exec_proc(s, a));
	debug_print("node not a proc...\n");
	if (AST_NODE_CMD == p_get_type(a))
		return (cmd_exec_simple(s, a)); // this one does not recurse
	debug_print("node not a cmd...\n");
	if (AST_NODE_PIPELINE == p_get_type(a))
		return (cmd_exec_pipe(s, a));
	debug_print("node not a pipe...\n");
	if (AST_NODE_LOG == p_get_type(a))
		return (cmd_exec_log(s, a));
	debug_print("ERR unknown node...\n");
	return (1);
}

/* CMD EXECUTE
 * Recursively executes the AST from Parser
 * Sets current_cmd in State then calls 
 * top 'switchboard' func
 * Returns last exit_code from input
 * 
 * Setting cmd in State allows multiple accesses
 * for expansions, globbing, etc.
 * 
 * The Command ADT stack tracks current AST depth
 * When a sub-method is called, the current node is
 * pushed. And when that sub-method returns, it is 
 * popped. Why?
 */
int	cmd_execute(t_state *s, t_ast_node *a)
{
	init_cmd(s, a);
	print_ast(a, 10);
	return (cmd_execute_full(s, a)); //TODO clean up exit code mgmt
}
