#include "../command/command.h"
#include "../parse/parse.h"
#include "log.h"

static void	_print_redirs(t_cmd *c)
{
	t_list	*redirs;
	int		i;

	i = -1;
	redirs = c_get_redirs(c);
	cprintf(YELLOW, "\tRedirects:%d\n", c_get_redc(c));
	if (c_get_redc(c) && redirs)
	{
		while (++i < c_get_redc(c))
		{
			if (p_get_fn(redirs->content))
				cprintf(YELLOW, "\t\t%d: _%s_\n", i,
					p_get_fn(redirs->content));
			else
				cprintf(YELLOW, "\t\t%d: (NULL)\n", i);
			redirs = redirs->next;
		}
	}
}

void	log_command_info(t_cmd *c, t_ast_node *a)
{
	int	i;

	if (LOGGING)
	{
		if (p_get_cmd(a))
			cprintf(YELLOW, "\tExecuting command: %s\n", p_get_cmd(a));
		else
			cprintf(RED, "\tExecuting command: (NULL)\n");
		cprintf(YELLOW, "\tArguments:\n");
		i = -1;
		if (c_get_argv(c))
		{
			while (++i < c_get_argvc(c))
				cprintf(YELLOW, "\t  argv[%d]: _%s_\n", i,
					c_get_argv(c)[i]);
		}
		if (p_get_argc(a) > 0 && c_get_argv(c)
			&& c_get_argv(c)[p_get_argc(a)] == NULL)
			cprintf(YELLOW, "\t  argv[%d]: (NULL)\n", p_get_argc(a));
		_print_redirs(c);
	}
	(void)i;
}

void	lgprint(const char *s, ...)
{
	va_list	args;

	if (LOGGING && isatty(STDERR_FILENO))
	{
		va_start(args, s);
		fprintf(stderr, CYAN "[LOG] ");
		vfprintf(stderr, s, args);
		fprintf(stderr, CYAN RESET);
		va_end(args);
		fflush(stderr);
	}
}

void	cprintf(const char *color, const char *format, ...)
{
	va_list	args;

	if (LOGGING && isatty(STDERR_FILENO))
	{
		va_start(args, format);
		fprintf(stderr, "%s", color);
		vfprintf(stderr, format, args);
		fprintf(stderr, "%s", RESET);
		va_end(args);
		fflush(stderr);
	}
}

void	err(const char *s)
{
	if (LOGGING)
		fprintf(stderr, RED "%s" RESET, s);
	else
		(void)s;
}
