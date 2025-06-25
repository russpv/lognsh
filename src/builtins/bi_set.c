#include "bi_int.h"

#define CMD_NAME "set"

static void print_set_usage(void)
{
	const int	fd = STDERR_FILENO;
	const char *pre = ": " ;

	write(fd, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(fd, pre, ft_strlen(pre));
	ft_putstr_fd("Usage: set <option>.\n", fd);
	ft_putstr_fd(" Available options:\n", fd);
	ft_putstr_fd("   'vdebug'      Enable verbose logging mode\n", fd);
	ft_putstr_fd("   'debug'       Enable debug mode\n", fd);
	ft_putstr_fd("   'debugoff'    Disable debug mode\n", fd);
	ft_putstr_fd("   'log'         Enable basic logging\n", fd);
	ft_putstr_fd("   'logoff'      Disable logging\n", fd);
}

static void print_unrec_option(void)
{
	const int	fd = STDERR_FILENO;
	const char *pre = ": " CMD_NAME;

	write(fd, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(fd, pre, ft_strlen(pre));
	ft_putstr_fd(" unrecognized option\n", fd);
}

static void	print_status(void)
{
	const int	fd = STDOUT_FILENO;
	const int	dlvl = get_debug_level();
	
	if (DEBUG_OFF == dlvl)
		ft_putstr_fd(MAGENTA " Debug logging is now OFF\n" RESET, fd);
	else if (DEBUG_LOG == dlvl)
		ft_putstr_fd(MAGENTA " Logging is now ON\n"RESET, fd);
	else if (DEBUG_ONE == dlvl)
		ft_putstr_fd(MAGENTA " Debug logging is now ON\n"RESET, fd);
	else if (DEBUG_VER == dlvl)
		ft_putstr_fd(MAGENTA " Debug verbose logging is now ON\n"RESET, fd);
	else if (DEBUG_VVR == dlvl)
		ft_putstr_fd(MAGENTA " Debug very verbose logging is now ON\n"RESET, fd);
}

int	bi_set(t_state *s, char **argv, int argc)
{
	(void)s;
	if (1 == argc)
		return (print_set_usage(), ERR_GENERAL);
	else if (0 == ft_strncmp(argv[1], LOGON, ft_strlen(DEBUGON) + 1))
		return (set_debug_level(DEBUG_LOG), print_status(), 0);
	else if (0 == ft_strncmp(argv[1], DEBUGON, ft_strlen(DEBUGON) + 1))
		return (set_debug_level(DEBUG_ONE), print_status(), 0);
	else if (0 == ft_strncmp(argv[1], DEBUGHI, ft_strlen(DEBUGON) + 1))
		return (set_debug_level(DEBUG_VER), print_status(), 0);		
	else if (0 == ft_strncmp(argv[1], DEBUGOFF, ft_strlen(DEBUGOFF) + 1))
		return (set_debug_level(DEBUG_OFF), print_status(), 0); 
	else if (0 == ft_strncmp(argv[1], LOGOFF, ft_strlen(DEBUGOFF) + 1))
		return (set_debug_level(DEBUG_OFF), print_status(), 0); 
	else
	{
		print_unrec_option();
		print_set_usage();
	}
	return (0);
}
