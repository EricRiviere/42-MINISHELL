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
#include <sys/wait.h>

//------------------ TOKEN TYPE ENUM
typedef enum    s_type
{
    WORD,
    QUOTED,
    OPERATOR
}   t_type;
//------------------
//------------------ OPERATOR TYPE ENUM
typedef enum    s_operator
{
    INPUT,
    OUTPUT,
    HEREDOC,
    APPEND,
    PIPE
}   t_operator;
//------------------
//------------------ TOKEN STRUCT
typedef struct  s_token t_token;

struct s_token
{
    char    *value;
    short   is_quote;
    short   expand;
    int     has_space;
    t_type  type;
    t_token *next;
    t_token *prev;
}; 
//------------------
//------------------ TOKEN ARGUMENTS STRUCT
typedef struct  s_args
{
    char    *line;
    int     *i;
    t_token **tkn_lst;
    int     spaces;
}   t_args;
//------------------
//-------------------ENV STRUCT
typedef struct s_env
{
    char    *key;
    char    *value;
    struct s_env *next;
}   t_env;
//------------------
//------------------ COMANDS STRUCTS TEST
typedef struct s_redir
{
    char    **operator;
    char    **file;
}   t_redir;


typedef struct s_command
{
    char    *cmd;
    char    **args;
    t_redir *redirections;
    int     redir_error;
    int     fd_in;
    int     fd_out;
}   t_command;
//------------------
//------------------ EXPAND ARGUMENTS STRUCT
typedef struct s_expand_args {
    t_token *token;
    t_env *env_lst;
    char *expand;
    int i;
    char *temp;
    char *sub_expand;
    int start;
} t_expand_args;
//------------------
//------------------ UTILS FUNCTIONS
char    *ft_strndup(const char *str, size_t len);
int is_space(char c);
int is_special_char(char c);
//------------------ ENVIROMENT FUNCTIONS
void    free_env_list(t_env *env_list);
void    add_env_variable(t_env **env_list, char *key, char *value);
t_env *init_env_list(char **env);
void    print_env_list(t_env *env_list);
char    *get_env_value(char *str, t_env *env_list);
//------------------ TOKENIZE FUNCTIONS
t_args  *pass_args(char *line, int *i, t_token **tkn_lst, int spaces);
t_token *init_token(t_type type);
void    add_token(t_token **tkn_lst, t_token *token);
void    free_tkn_lst(t_token *tkn_lst);
t_token    *parse_quote(t_args *args, char quote, int start, int end);
t_token   *manage_quote(t_args *args, int start);
t_token *parse_operator(t_operator operator, int spaces);
t_operator get_operator_type(const char *line, int i);
int create_operator_token(t_args *args, t_operator operator, int increment);
t_token *manage_operator(t_args *args);
t_token *manage_word(t_args *args);
void skip_spaces(const char *line, int *i, int *spaces);
int process_token(t_args *args);
t_token *tokenize(char *line);
//------------------ SINTAXIS FUNCTIONS
int syntax_check(t_token *tkn_lst);
//------------------ EXPANSION FUNCTIONS
char *ft_strjoin_free(char *s1, char *s2);
int is_valid_env_char(char c);
char *expand_value(t_token *token, t_env *env_lst);
void expand_variables(t_token *token, t_env *env_lst);
//------------------ COMMAND FUNCTIONS
//void    commands(t_token *tkn_lst);
t_command **commands(t_token *tkn_lst);
void preprocess_tokens(t_token **tkn_lst);
void free_cmd_list(t_command **cmd_list);
void execute_cmd(t_command **cmd, t_env **env);
//------------------ PRINT FUNCTIONS
void print_commands(char *line, t_command **cmd_list);
void print_tokens(char *line, t_token *tkn_lst);
//------------------ HEREDOC
void process_heredoc(t_token *heredoc_token);
//------------------ BUILTINS
void    manage_builtins(t_command **cmd, t_env **env);
int is_builtin(t_command *cmd);
//------------------ EXPORT - UNSET FUNCTIONS
t_env *get_var(t_env **env, char *key);
void    cu_env_var(t_env **env, char *key, char *value);
void export_new_var(t_command **cmd, t_env **env);
void delete_env_var(t_command **cmd, t_env **env);
//------------------ CD FUNCTION
void change_dir(const char *path, t_env **env);
//------------------ PWD FUNCTION
void    printf_pwd(t_env **env);
//------------------ ECHO FUNCTION
void get_echo(t_command **cmd);
int get_cmd_num(t_command **cmd);
#endif