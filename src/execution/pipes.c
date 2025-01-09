#include "minishell.h"
void   manage_pipes(t_command **cmd, t_env **env)
{
    int i = 0;
    int j = 0;
    int n = get_cmd_num(cmd);
    int *id = malloc(sizeof(int) * n); 
    while (i <= n)
    {
        id[j] = fork();
        // proteger
        // if ()
        // else ()
    }
    
}