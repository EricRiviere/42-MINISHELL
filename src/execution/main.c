#include "minishell.h"
// void ctrl_c(int signal)
// {
//     if (signal == SIGINT)
//     {
//         printf("\nminishell> ");
//     }
// }

int main(int argc, char **argv, char **env)
{
    char    *line;
    t_token *tkn_lst;
    t_token *curr_tkn;
    t_env   *env_lst;
    t_command **cmd_list;

    // signal(SIGINT, ctrl_c);
    // signal(SIGQUIT, SIG_IGN);
    // parent_signals();
    (void)argv;
    if (argc != 1)
    {
        printf("Wrong number of arguments\n");
        return (1);
    }
    env_lst = init_env_list(env);
    parent_signals();
    while (1)
    {
        // parent_signals();
        line = readline("minishell> ");
        if (!line)
            break;
        if (line)
        {
            add_history(line);
            tkn_lst = tokenize(line);
            if (tkn_lst && !syntax_check(tkn_lst))
            {
                free_tkn_lst(tkn_lst);
                free(line);
                continue;
            }
            if (tkn_lst)
            {
                curr_tkn = tkn_lst;
                while (curr_tkn)
                {
                    if (ft_strncmp(curr_tkn->value, "<<", 2) == 0)
                        curr_tkn->hd_fd = process_heredoc(curr_tkn);
                    expand_variables(curr_tkn, env_lst);
                    curr_tkn = curr_tkn->next;
                }   
                preprocess_tokens(&tkn_lst);
                cmd_list = commands(tkn_lst);
                execute_pipes(cmd_list, &env_lst);
                //execute_cmd(cmd_list, &env_lst);
                //print_commands(line, cmd_list);
               // print_tokens(line, tkn_lst);
                free_cmd_list(cmd_list);
            }
            free_tkn_lst(tkn_lst);
        }
        free(line);
    }
    free_env_list(env_lst);
    return (0);
}