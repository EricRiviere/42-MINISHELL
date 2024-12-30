#include "minishell.h"

void    print_declared_env(t_env **env_list)
{
    t_env *curr_var;

    curr_var = *env_list;
    while(curr_var)
    {
        if (curr_var->value[0] != '\0')
            printf("declare -x %s=\"%s\"\n", curr_var->key, curr_var->value);
        else
            printf("declare -x %s\n", curr_var->key);
        curr_var = curr_var->next;
    }
}

void    print_enviroment(t_env **env_list)
{
    t_env *curr_var;

    curr_var = *env_list;
    while(curr_var)
    {
        if (curr_var->value[0] != '\0')
            printf("%s=%s\n", curr_var->key, curr_var->value);
        curr_var = curr_var->next;
    }
}

void    manage_builtins(t_command **cmd, t_env **env)
{
    int i = 0;

    while (cmd[i])
    {
        if (ft_strncmp(&cmd[0]->cmd[0], "env", ft_strlen("env")) == 0 
            && ft_strlen("env") == ft_strlen(&cmd[0]->cmd[0]))//Caso impresion env
            print_enviroment(env);
        else if (ft_strncmp(&cmd[0]->cmd[0], "export", ft_strlen("export")) == 0 
            && ft_strlen("export") == ft_strlen(&cmd[0]->cmd[0]))//Caso export tiene que ser el 0
        {
            if (!cmd[0]->args[0])
                print_declared_env(env);//Impresion con export
            else
                export_new_var(cmd, env);
        }
        else if (ft_strncmp(&cmd[0]->cmd[0], "unset", ft_strlen("unset")) == 0 
            && ft_strlen("unset") == ft_strlen(&cmd[0]->cmd[0]))//Caso unset tiene que ser el 0
            delete_env_var(cmd, env);
        i++;
    }
}