/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:56:47 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 20:52:18 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
//readline --> compilar con -lreadline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
//------------------
# include <stdlib.h> //malloc
# include <unistd.h> // write
# include "libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <stddef.h>
# include <signal.h>
//------------------ TOKEN TYPE ENUM
typedef enum s_type
{
	WORD,
	QUOTED,
	OPERATOR
}	t_type;
//------------------
//------------------ OPERATOR TYPE ENUM
typedef enum s_operator
{
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	PIPE
}	t_operator;
//------------------
//------------------ TOKEN STRUCT
typedef struct s_token	t_token;

struct s_token
{
	char	*value;
	short	is_quote;
	short	expand;
	int		has_space;
	int		hd_fd;
	t_type	type;
	t_token	*next;
	t_token	*prev;
};
//------------------
//------------------ TOKEN ARGUMENTS STRUCT
typedef struct s_args
{
	char	*line;
	int		*i;
	t_token	**tkn_lst;
	int		spaces;
}	t_args;
//------------------
//-------------------ENV STRUCT
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;
//------------------
//------------------ COMANDS STRUCTS TEST
typedef struct s_redir
{
	char	**operator;
	char	**file;
}	t_redir;

typedef struct s_command
{
	char		*cmd;
	char		**args;
	t_redir		*redirections;
	int			redir_error;
	int			status;
	int			fd_in;
	int			fd_out;
}	t_command;
//------------------
//------------------ EXPAND ARGUMENTS STRUCT
typedef struct s_expand_args
{
	t_token		*token;
	t_env		*env_lst;
	char		*expand;
	int			i;
	char		*temp;
	char		*sub_expand;
	int			start;
}	t_expand_args;
//------------------
//------------------ PIPE STRUCT
typedef struct s_pipe_data
{
	int		cmd_num;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;
}	t_pipe_data;
//------------------
//------------------ UTILS FUNCTIONS
char		*ft_strndup(const char *str, size_t len);
int			is_space(char c);
int			is_special_char(char c);
//------------------ ENVIROMENT FUNCTIONS
void		free_env_list(t_env *env_list);
void		add_env_variable(t_env **env_list, char *key, char *value);
t_env		*init_env_list(char **env);
void		print_env_list(t_env *env_list);
char		*get_env_value(char *str, t_env *env_list);
//------------------ TOKENIZE FUNCTIONS
t_args		*pass_args(char *line, int *i, t_token **tkn_lst, int spaces);
t_token		*init_token(t_type type);
void		add_token(t_token **tkn_lst, t_token *token);
void		free_tkn_lst(t_token *tkn_lst);
t_token		*parse_quote(t_args *args, char quote, int start, int end);
t_token		*manage_quote(t_args *args, int start);
t_token		*parse_operator(t_operator operator, int spaces);
// t_operator get_operator_type(const char *line, int i);
int			create_operator_token(t_args *args,
				t_operator operator, int increment);
t_token		*manage_operator(t_args *args);
t_token		*manage_word(t_args *args);
void		skip_spaces(const char *line, int *i, int *spaces);
int			process_token(t_args *args);
t_token		*tokenize(char *line);
char		*return_error(char *str);
//------------------ SINTAXIS FUNCTIONS
int			syntax_check(t_token *tkn_lst);
//------------------ EXPANSION FUNCTIONS
char		*ft_strjoin_free(char *s1, char *s2);
int			is_valid_env_char(char c);
char		*expand_value(t_token *token, t_env **env_lst);
void		expand_variables(t_token *token, t_env **env_lst);
void		handle_literal_text(t_expand_args *args);
void		handle_variable_expansion(t_expand_args *args);
//------------------ COMMAND FUNCTIONS
void		concat_tokens(t_token **curr, t_token **prev);
void		preprocess_tokens(t_token **tkn_lst);
void		manage_redirection_cmd(t_token **curr_tkn, t_redir *redir,
				int *op_index);
void		manage_word_quote_cmd(t_command *cmd, t_token **curr_tkn,
				int *arg_index);
void		process_tokens(t_command *cmd, t_redir *redir, t_token **curr_tkn);
// t_command *allocate_command_resources(void);
// t_command *init_command(void);
void		add_redirection(t_redir *redir, t_token *curr_tkn, int *op_index);
void		close_fd_if_open(int *fd);
int			process_redirections(t_command *cmd, t_redir *redir);
int			process_input_fd(t_command *cmd, const char *file);
int			process_output_fd(t_command *cmd, const char *file);
int			process_append_fd(t_command *cmd, const char *file);
void		free_failed_commands(t_command **cmd_list, int cmd_index);
void		free_command_resources(t_command *cmd);
// t_command	**commands(t_token *tkn_lst);
void		free_redir(t_redir *redir);
void		free_single_cmd(t_command *cmd);
void		free_cmd_list(t_command **cmd_list);
//------------------ EXECUTION FUNCTIONS
char		*find_path(t_env *env);
void		handle_absolute_path(t_command **cmd, char **new_arr);
void		handle_relative_path(t_command **cmd, t_env **env, char **new_arr);
char		*build_full_path(char *dir, char *cmd);
void		execute_path(t_command **cmd, t_env **env, char **new_arr);
int			arr_len(char **cmds);
char		**new_args(t_command **cmd);
void		free_array(char **arr);
char		**create_env_array(t_env *env_list, int list_len);
char		**env_arr(t_env *env_list);
int			get_cmd_num(t_command **cmd);
void		cmd_not_found(t_command **cmd, char **new_arr);
void		execute_cmd(t_command **cmd, t_env **env);
//------------------ HEREDOC
int			process_heredoc(t_token *heredoc_token);
//------------------ BUILTINS
void		manage_builtins(t_command *cmd, t_env **env);
int			is_builtin(t_command *cmd);
//------------------ EXPORT - UNSET FUNCTIONS
int			is_valid_key(char c);
t_env		*get_var(t_env **env, char *key);
void		add_env_variable_with_null(t_env **env_list, char *key);
void		cu_env_var(t_env **env, char *key, char *value);
char		*extract_key(char *arg);
char		*extract_value(char *arg);
void		process_argument(char *arg, t_env **env);
void		export_new_var(t_command **cmd, t_env **env);
void		delete_env_var(t_command **cmd, t_env **env);
void		print_declared_env(t_env **env_list);
//------------------ CD FUNCTION
void		change_dir(t_command *cmd, t_env **env);
//------------------ PWD FUNCTION
void		printf_pwd(t_env **env);
//------------------ ECHO FUNCTION
void		get_echo(t_command *cmd);
int			get_cmd_num(t_command **cmd);
//------------------ EXIT
void		ft_exit(t_command *cmd);
//------------------ PIPES
void		execute_pipes(t_command **cmds, t_env **env);
//------------------ SIGNALS
void		parent_signals(void);
void		output_signals(int sig);
void		here_signals(int sig);
void		handle_signaled(int *status, int signal);
void		double_free_char(char *s1, char *s2);
int			get_status(int flag, int value);
int			get_break_it(int flag, int value);
void		break_it(int signal);
//------------------ DEBUG
void		print_tokens(char *line, t_token *tkn_lst);
void		print_commands(char *line, t_command **cmd_list);

//------ functions de norminette
t_operator	get_operator_type(const char *line, int i);
t_command	*allocate_command_resources(void);
t_command	*init_command(void);
t_command	**commands(t_token *tkn_lst);
#endif
