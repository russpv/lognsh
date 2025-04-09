#include "execute_int.h"

#define LOGMSG_EREDIR_DONE "\tRedirect: dup2 from %d to %d\n"

static int	_do_open(char *topath, int from, bool append, int *fd)
{
	struct stat	statbuf;

	if (access(topath, F_OK) == 0 && stat(topath, &statbuf) == -1)
		return (print_perror("stat"), -ERR_STAT);
	else if (from != STDIN_FILENO && access(topath, F_OK) == 0
		&& S_ISDIR(statbuf.st_mode))
		return (print_is_dir(topath), -ERR_REDIR);
	else if (from == STDIN_FILENO && access(topath, F_OK) == 0
		&& S_ISDIR(statbuf.st_mode))
		*fd = open(topath, O_RDONLY | O_DIRECTORY);
	else if (from == STDIN_FILENO)
		*fd = open(topath, O_RDONLY | O_CREAT, 0644);
	else if (from != STDIN_FILENO && append)
		*fd = open(topath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (from != STDIN_FILENO)
		*fd = open(topath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		*fd = -1;
	return (0);
}

/* Does the correct open() and returns new fd
 * or negative error codes.
 * If a directory:
 *   Inputs -- throws no error only if no perms
 *   Outputs -- throws error
 * If DNE or no perms: throws error
 * [Deprecated] if input file can't be read, opens
 *	/dev/null/ to allow cmd to run
 */
static inline int	_redirect_logic(char *topath, int from, bool append)
{
	int			res;
	int			fd;

	fd = 0;
	lgprint(_MOD_ ":%s got path:%s, from:%d app:%d\n", __FUNCTION__, topath,
		from, append);
	if (from == STDIN_FILENO && access(topath, F_OK) == -1)
	{
		print_redirect_warning(topath);
		print_dne(topath);
		errno = EACCES;
		return (-ERR_REDIR);
	}
	else
	{
		res = _do_open(topath, from, append, &fd);
		if (0 != res)
			return (res);
	}
	if (fd < 0)
		return (print_redirect_warning(topath), perror(EMSG_OPEN), fd);
	dprint("%s: %s: fd:%d\n", _MOD_, __FUNCTION__, fd);
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
	cprintf(GREEN, LOGMSG_EREDIR_DONE, from, fd);
	if (dup2(fd, from) == -1)
		return (perror(EMSG_DUP2), ERR_DUP2);
	if (0 != close(fd))
		return (perror(EMSG_CLOSE), ERR_CLOSE);
	return (0);
}
