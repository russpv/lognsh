/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:56 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:57 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_INT_H
# define STATE_INT_H

# include "state.h"

# define PROMPT_KEY "PROMPT"
# define LVL_KEY "SHLVL"
# define CWD_KEY "PWD"
# define _MOD_ "State"

struct							s_global_state
{
	char						**envp;
	t_env						*sh_env_list;
	char						*pwd;
	int							current_exit_code;
	int							error_code;
	bool						got_heredoc;
	char						*input;

	t_list						*tmp_tok_lst;
	char						*tmp;
	int							tmp_flag;

	char						*prompt;
	char						**path;

	t_parser					*current_parser;
	t_lex						*current_lexer;
	t_cmd						*current_cmd;

	t_destroy_fn				destroy_parser;
	t_destroy_fn				destroy_lexer;
	t_destroy_fn				destroy_command;
	t_destroy_fn				destroy_token;

	t_mem_mgr					mem_mgr;

	t_cmd_fns					*cmd_fns;
	t_lex_fns					*lex_fns;
	t_parse_fns					*parse_fns;
};

void	init_env_vars(t_state *s);

#endif
