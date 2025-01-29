/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:51 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:28:37 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BI_H
# define BI_H

# define BI_ECHO "echo"
# define BI_CD "cd"
# define BI_PWD "pwd"
# define BI_EXPORT "export"
# define BI_UNSET "unset"
# define BI_ENV "env"
# define BI_EXIT "exit"
# define BI_COUNT 7


typedef int (*t_builtin_fn)(char **args); //RKP: last exit code should be set by caller

t_builtin_fn	get_builtin(char *command);

/* Bash Manual
 *
 * All of the Bash builtins return an exit status of zero if
 * they succeed and a non-zero status on failure, so they may
 * be used by the conditional and list constructs. All builtins
 * return an exit status of 2 to indicate incorrect usage,
 * generally invalid options or missing arguments.
 *
 */

/*
cd
	cd [-L|[-P [-e]] [-@] [directory]

	Change the current working directory to directory. If directory is not supplied,
		the value of the HOME shell variable is used. If the shell variable CDPATH exists,
		it is used as a search path: each directory name in CDPATH is searched for directory,
		with alternative directory names in CDPATH separated by a colon (‘:’). If directory begins with a slash,
		CDPATH is not used.

	The
		-P option means to not follow symbolic links: symbolic links are resolved while cd is traversing directory and before processing an instance of ‘..’ in directory.

	By default, or when the -L option is supplied,
		symbolic links in directory are resolved after cd processes an instance of ‘..’ in directory.

	If ‘..’ appears in directory,
		it is processed by removing the immediately preceding pathname component,
		back to a slash or the beginning of directory.

	If the -e option is supplied with
		-P and the current working directory cannot be successfully determined after a successful directory change,
		cd will return an unsuccessful status.

	On systems that support it, the
		-@ option presents the extended attributes associated with a file as a directory.

	If directory is ‘-’,
		it is converted to $OLDPWD before the directory change is attempted.

	If a non-empty directory name from CDPATH is used,
		or if ‘-’ is the first argument, and the directory change is successful,
		the absolute pathname of the new working directory is written to the standard output.

	If the directory change is successful,
		cd sets the value of the PWD environment variable to the new directory name,
		and sets the OLDPWD environment variable to the value of the current working directory before the change.

	The return status is zero if the directory is successfully changed,
		non-zero otherwise.
*/

/*
exit

	exit [n]

	Exit the shell,
		returning a status of n to the shell’s parent. If n is omitted,
		the exit status is that of the last command executed. Any trap on EXIT is executed before the shell terminates.
*/

/*
export

	export [-fn] [-p] [name[=value]]

	Mark each name to be passed to child processes in the environment. If the
		-f option is supplied,
		the names refer to shell functions; otherwise the names refer to shell variables. The
		-n option means to no longer mark each name for export. If no names are supplied,
		or if the -p option is given,
		a list of names of all exported variables is displayed. The
		-p option displays output in a form that may be reused as input. If a variable name is followed by =value,
		the value of the variable is set to value.

	The return status is zero unless an invalid option is supplied,
		one of the names is not a valid shell variable name, or
		-f is supplied with a name that is not a shell function.
*/

/*
pwd

	pwd [-LP]

	Print the absolute pathname of the current working directory. If the
		-P option is supplied,
		the pathname printed will not contain symbolic links. If the
		-L option is supplied,
		the pathname printed may contain symbolic links. The return status is zero unless an error is encountered while determining the name of the current directory or an invalid option is supplied.
*/

/*
unset

	unset [-fnv] [name]

	Remove each variable or function name. If the -v option is given,
		each name refers to a shell variable and that variable is removed. If the
		-f option is given, the names refer to shell functions,
		and the function definition is removed. If the -n option is supplied,
		and name is a variable with the nameref attribute,
		name will be unset rather than the variable it references.
		-n has no effect if the
		-f option is supplied. If no options are supplied,
		each name refers to a variable; if there is no variable by that name,
		a function with that name, if any,
		is unset. Readonly variables and functions may not be unset. Some shell variables lose their special behavior if they are unset; such behavior is noted in the description of the individual variables. The return status is zero unless a name is readonly or may not be unset.
*/

/*
echo

	echo [-neE] [arg …]

	Output the args, separated by spaces,
		terminated with a newline. The return status is 0 unless a write error occurs. If
		-n is specified, the trailing newline is suppressed. If the
		-e option is given,
		interpretation of the following backslash-escaped characters is enabled. The
		-E option disables the interpretation of these escape characters,
		even on systems where they are interpreted by default. The xpg_echo shell option may be used to dynamically determine whether or not echo expands these escape characters by default. echo does not interpret -- to mean the end of options.

	echo interprets the following escape sequences:

	\a

		alert (bell)
	\b

		backspace
	\c

		suppress further output
	\e
	\E

		escape
	\f

		form feed
	\n

		new line
	\r

		carriage return
	\t

		horizontal tab
	\v

		vertical tab
	\\

		backslash
	\0nnn

		the eight-bit character whose value is the octal value nnn (zero to three octal digits)
	\xHH

		the eight-bit character whose value is the hexadecimal value HH (one or two hex digits)
	\uHHHH

		the Unicode (ISO/IEC 10646) character whose value is the hexadecimal value HHHH (one to four hex digits)
	\UHHHHHHHH

		the Unicode (ISO/IEC 10646) character whose value is the hexadecimal value HHHHHHHH (one to eight hex digits)

*/
#endif