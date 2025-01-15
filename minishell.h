#ifndef MINISHELL_H
# define MINISHELL_H

#include "signal.h"
#include "state.h" // << parse.h, command.h, execute.h, env.h
//#include "parser.h" // << command.h (builds token string)
//#include "command.h" //<< tree.h (builds the tree)
//#include "execute.h" // << command.h (forks), 
//#include 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <termios.h>
#include <errno.h>
#include <ctype.h>


#endif
