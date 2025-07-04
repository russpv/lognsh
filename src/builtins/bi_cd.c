#include "bi_int.h"

#define CMD_NAME "cd"
#define PWD_KEY "PWD"
#define EMSG_OLDPWDNOTSET "OLDPWD is not set.\n"
#define EMSG_NOFILE "No such file or directory\n"
#define EMSG_TOOMANYARGS "too many arguments\n"
#define EMSG_NOHOME "HOME is not set.\n"
#define EMSG_BADMALLOC "Memory allocation failed.\n"

static int	_go_up(char *pwdbuf)
{
	size_t	len;
	size_t	del_len;
	char	*marker;

	marker = ft_strrchr(pwdbuf, '/');
	if (NULL == marker)
		return (ERR_GENERAL);
	len = (size_t)(marker - pwdbuf);
	if (0 == len)
		return (ERR_GENERAL);
	if (len > INPUT_BUF_LIM - 1)
		return (ERR_BUFFLOW);
	del_len = ft_strnlen(pwdbuf, INPUT_BUF_LIM) - len;
	ft_memset(pwdbuf + len, 0, del_len);
	return (0);
}

// Returns NULL if invalid abs path
char	*make_absolute(t_state *s, const char *relpath)
{
	char	*pwd;
	char	buf[INPUT_BUF_LIM];

	ft_memset(buf, 0, INPUT_BUF_LIM);
	pwd = get_env_val(s, PWD_KEY);
	ft_memmove(buf, pwd, ft_strlen(pwd));
	while (*relpath && 0 == ft_strncmp(relpath, "..", 2))
	{
		relpath += 2;
		if (0 != _go_up(buf))
			return (NULL);
		if (0 == ft_strncmp(relpath, "/", 1))
			relpath++;
	}
	buf[ft_strnlen(buf, INPUT_BUF_LIM)] = '/';
	return (ft_strjoin_mem(&get_mem(s)->list, get_mem(s)->f, buf, relpath));
}

// Dupes target (in case it comes from OLDPWD/PWD) and sends to chdir()
static int	_change_dir(t_state *s, const char *target)
{
	char		*new_dir;
	char		*alt_dir;
	const char	*old_pwd = env_find_value(OLDPWD_KEY, get_env_list(s));

	if (!target)
		return (ERR_GENERAL);
	new_dir = ft_strdup_mem(get_mem(s), target);
	if (0 == ft_strcmp(target, "-"))
		if (!old_pwd || ft_strcmp(old_pwd, "") == 0)
			return (print_custom_err(CMD_NAME, EMSG_OLDPWDNOTSET), 1);
	set_oldpwd(s, CMD_NAME);
	if (0 == chdir(new_dir))
		return (set_pwd(s), 0);
	if (NULL == ft_strchr(new_dir, '/'))
	{
		alt_dir = make_absolute(s, new_dir);
		if (0 != chdir(alt_dir))
		{
			get_mem(s)->dealloc(&get_mem(s)->list, alt_dir);
			return (print_custom_err_err(CMD_NAME, target, strerror(errno)),
				ERR_GENERAL);
		}
	}
	set_pwd(s);
	return (0);
}

/* Change the current directory to DIR.  The default DIR is the value of the
 * HOME shell variable. If DIR is "-", it is converted to $OLDPWD.
 * Note: return value becomes exit code.
 */
int	bi_cd(t_state *s, char **args, int argc)
{
	char	*target;

	if (!s || !args)
	{
		print_custom_err(CMD_NAME, EMSG_INVLD);
		return (ERR_ARGS);
	}
	if (argc == 1)
		target = env_find_value("HOME", get_env_list(s));
	else if (argc == 2)
		target = args[1];
	else
	{
		print_custom_err(CMD_NAME, EMSG_TOOMANYARGS);
		return (EX_ERNDM);
	}
	if (!target || ft_strcmp(target, "") == 0)
	{
		print_custom_err(CMD_NAME, EMSG_NOHOME);
		return (ERR_GENERAL);
	}
	return (_change_dir(s, target));
}
