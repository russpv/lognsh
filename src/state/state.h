#ifndef STATE_H
# define STATE_H

# include "../../include/libft.h"
# include "../builtins/env/env.h"
# include "../data_structures/stack.h"
# include "../mem/mem.h"
# include "../signal/signal.h"
# include "../utils/debug.h"
# include "../utils/log.h"
# include "error.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>

# define OLDPWD_KEY "OLDPWD"
# define PATH_KEY "PATH"

# ifdef TESTMODE
#  define TESTFLAG 1
# else
#  define TESTFLAG 0
# endif


/* STATE ADT
** A centralized singleton-like pattern for tracking global state
** including environment, execution state.
*/

typedef struct s_global_state	t_state;

typedef void					(*t_destroy_fn)(t_mem_mgr *m, void **instance);
typedef void					(*t_attach_fn)(void *instance);

/* Duplicate declarations instead of includes */
typedef struct s_parser			t_parser;
typedef struct s_lex			t_lex;
typedef struct s_cmd			t_cmd;
typedef struct s_env			t_env;

/* For dependency injections */

/* Command injector */
typedef char					*(*c_get_fullpath_fn)(t_cmd *c);
typedef char					**(*c_get_argv_fn)(t_cmd *c);
typedef t_int_stack				*(*c_get_ctxtst_fn)(t_cmd *c);
typedef t_ast_node				*(*c_get_node_fn)(t_cmd *c);
typedef int						(*c_get_cmdc_fn)(const t_cmd *c);
typedef int						(*c_get_argvc_fn)(const t_cmd *c);
typedef const int				**(*c_get_fildes_fn)(const t_cmd *c);
typedef int						(*save_redirs_fn)(t_cmd *c);
typedef int						(*restore_redirs_fn)(t_cmd *c);

typedef struct s_command_functions
{
	c_get_argv_fn				c_get_argv;
	c_get_argvc_fn				c_get_argvc;
	c_get_cmdc_fn				c_get_cmdc;
	c_get_node_fn				c_get_node;
	c_get_fullpath_fn			c_get_fullpath;
	c_get_ctxtst_fn				c_get_ctxtst;
	c_get_fildes_fn				c_get_fildes;
	save_redirs_fn				save_redirs;
	restore_redirs_fn			restore_redirs;
}								t_cmd_fns;

t_cmd_fns						*get_cmd_fns(t_state *s);
t_cmd_fns						*init_cmd_fns(t_state *s);

/* Lexer injector */
typedef char					*(*lex_get_eof_fn)(t_lex *lexer);
typedef int						(*lex_get_lines_fn)(t_lex *lexer);
typedef int						(*match_heredoc_fn)(t_mem_mgr *m, t_lex *l);
typedef size_t					(*write_heredoc_fn)(int fd, t_lex *l);
typedef size_t					(*read_heredoc_fn)(int fd, t_lex *l);

typedef struct s_lexer_functions
{
	lex_get_eof_fn				lex_get_eof;
	lex_get_lines_fn			lex_get_lines;
	match_heredoc_fn			lex_match_heredoc;
	write_heredoc_fn			write_heredoc;
	read_heredoc_fn				read_heredoc;
}								t_lex_fns;

t_lex_fns						*get_lex_fns(t_state *s);
t_lex_fns						*init_lex_fns(t_state *s);

/* Parse injector */
typedef int						(*p_do_redirections_fn)(t_ast_node *a);

typedef struct s_parse_functions
{
	p_do_redirections_fn		p_do_redirections;
}								t_parse_fns;

t_parse_fns						*get_parse_fns(t_state *s);
t_parse_fns						*init_parse_fns(t_state *s);


/* Methods */
t_state							*init_state(char **envp);
void							destroy_state(t_state **state);

void							set_exit_status(t_state *state, int value);
void							set_error(t_state *state, int code);
void							set_parser(t_state *state, t_parser *p);
void							set_lexer(t_state *state, t_lex *l);
void							set_input(t_state *s, char *input);
void							set_command(t_state *s, t_cmd *c);
void							set_pwd(t_state *s);
int								set_oldpwd(t_state *s, const char *caller);
void							set_tmp(t_state *s, char *str);
void							set_got_heredoc(t_state *s);
void							set_tmp_flag(t_state *s, int val);
void							set_path(t_state *s, const char *val);

int								*get_status(t_state *s);
char							*get_input(t_state *s);
t_cmd							*get_cmd(t_state *s);
t_list							**get_tmp_tok_list(t_state *s);
int								get_tmp_flag(t_state *s);

char							**get_envp(t_state *s);
char							*get_env_val(t_state *s, const char *key);
t_env							*get_env_list(t_state *s);
t_env							**get_env_list_ptr(t_state *s);
char							**get_path(t_state *s);

char							*get_tmp(t_state *s);
t_lex							*get_lexer(t_state *s);
char							*get_pwd(t_state *s);
char							*get_prompt(t_state *s);
bool							get_heredoc(t_state *s);

void							register_command_destroy(t_state *s,
									t_destroy_fn fn);
void							register_parser_destroy(t_state *s,
									t_destroy_fn fn);
void							register_lexer_destroy(t_state *s,
									t_destroy_fn fn);
void							register_token_destroy(t_state *s,
									t_destroy_fn fn);
void							s_free_tmp(t_state *s);
void							s_free_cmd(t_state *s);
void							s_free_cmd_lex_parse(t_state *state);

bool							has_sh_envp(t_state *s);

extern void						*myalloc(t_mem_node *head, size_t size);
extern void						myfree(t_mem_node *head, void *alloc);
void							exit_clean(t_mem_node *n, int exit_status,
									const char *caller, const char *errmsg);
t_mem_mgr						*get_mem(t_state *s);

#endif
