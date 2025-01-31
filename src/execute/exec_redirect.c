#include "execute_int.h"


/* Does the correct open() and returns new fd
 * If input file can't be opened, opens /dev/null/ to allow cmd 
 * to run otherwise and prints
 * warning as fish does
*/
static inline int	_redirect_logic(char *topath, int from, bool append)
{
	int	fd;

	if (from == STDIN_FILENO && access(topath, R_OK) == -1)
	{
		printf("warning: An error occurred while redirecting file '%s'\n", \
				topath);
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
	debug_print("_redirect_logic fd:%d\n", fd);
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
int	redirect(int *to, char *topath, int from, t_bool ifappend)
{
	int	fd;

	if (!to && (topath != NULL && *topath))
	{
		fd = _redirect_logic(topath, from, ifappend);
		if (fd < 0)
			return (-1);
	}
	else
		fd = *to;
	if (dup2(fd, from) == -1)
		return (-1);
	close(fd);
	return (from);
}
