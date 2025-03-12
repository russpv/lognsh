#include "execute_int.h"

/* Does the correct open() and returns new fd
 * If input file can't be opened, opens /dev/null/ to allow cmd
 * to run otherwise and prints
 * warning as fish does
 */
#define LOGMSG_EREDIR_DONE "\tRedirect: dup2 from %d to %d\n"
#define DBGMSG_EREDIR_FD _MOD_ ": %s: fd:%d\n"

static inline int	_redirect_logic(char *topath, int from, bool append)
{
	int	fd;

	if (from == STDIN_FILENO && access(topath, R_OK) == -1)
	{
		print_redirect_error(topath);
		fd = open("/dev/null", O_RDONLY);
		errno = EACCES;
	}
	else if (from == STDIN_FILENO)
		fd = open(topath, O_RDONLY | O_CREAT, 0644);
	else if (from == STDOUT_FILENO && append)
		fd = open(topath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (from == STDOUT_FILENO)
		fd = open(topath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = -1;
	if (fd < 0)
		return(perror(EMSG_OPEN), fd);
	debug_print(DBGMSG_EREDIR_FD, __FUNCTION__, fd);
	return (fd);
}

/* REDIRECT()
 * Encapsulates open() and dup2() and returns origin fd
 * or -1
 * Either:
 * 1. Opens input file for reading
 * 2. Opens output file for writing
 * And closes the duped fd.
 * Error message is FISH version
 *
 * Uses to-fd (e.g. pipe fds) unless NULL, then to-path
 * to: output fd
 * topath: output fd file path
 * from: input fd
 * append: for opt <</>> here_doc
 * from file is only ever stdin or stdout
 */
int	redirect(int *to, char *topath, int from, bool ifappend)
{
	int	fd;

	if (!to && (topath != NULL && *topath))
	{
		fd = _redirect_logic(topath, from, ifappend);
		if (fd < 0)
			return (fd);
	}
	else
		fd = *to;
	colored_printf(GREEN, LOGMSG_EREDIR_DONE, from, fd);
	if (dup2(fd, from) == -1)
		return (ERR_DUP2);
	if (0 != close(fd))
		return (perror(EMSG_CLOSE), ERR_CLOSE);
	return (from);
}
