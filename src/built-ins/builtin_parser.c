#include "minishell.h"

void    print_declared_env(t_env **env_list)
{
    t_env *curr_var;

    curr_var = *env_list;
    while(curr_var)
    {
        if (curr_var->value == NULL)
            printf("declare -x %s\n", curr_var->key);
        else if (curr_var->value[0] == '\0')
            printf("declare -x %s=\"\"\n", curr_var->key);
        else
            printf("declare -x %s=\"%s\"\n", curr_var->key, curr_var->value);
        curr_var = curr_var->next;
    }
}

void    print_enviroment(t_env **env_list)
{
    t_env *curr_var;

    curr_var = *env_list;
    while(curr_var)
    {
        if (curr_var->value != NULL && curr_var->value[0] != '\0')
            printf("%s=%s\n", curr_var->key, curr_var->value);
        curr_var = curr_var->next;
    }
}

void    manage_builtins(t_command **cmd, t_env **env)
{
    int i = 0;

    while (cmd[i])
    {
        if (&cmd[i]->cmd[0] == NULL)
            return ;
        else if (ft_strncmp(&cmd[i]->cmd[0], "env", ft_strlen("env")) == 0 
            && ft_strlen("env") == ft_strlen(&cmd[i]->cmd[0]))//Caso impresion env
            print_enviroment(env);
        else if (ft_strncmp(&cmd[i]->cmd[0], "cd", ft_strlen("cd")) == 0 
            && ft_strlen("cd") == ft_strlen(&cmd[i]->cmd[0]))//Caso cd
            {
                if (!cmd[0]->args || !cmd[0]->args[0])
                    change_dir(NULL, env);
                else if (cmd[0]->args[1])
                    ft_putstr_fd("minishell: cd: too many arguments", 2);
                else
                    change_dir(cmd[0]->args[0], env);
            }
        else if (ft_strncmp(&cmd[i]->cmd[0], "export", ft_strlen("export")) == 0 
            && ft_strlen("export") == ft_strlen(&cmd[i]->cmd[0]))//Caso export tiene que ser el 0
        {
            if (!cmd[0]->args[0])
                print_declared_env(env);//Impresion con export
            else
                export_new_var(cmd, i, env);
        }
        else if (ft_strncmp(&cmd[i]->cmd[0], "unset", ft_strlen("unset")) == 0 
            && ft_strlen("unset") == ft_strlen(&cmd[i]->cmd[0]))//Caso unset tiene que ser el 0
            delete_env_var(cmd, i, env);
        else if (ft_strncmp(&cmd[i]->cmd[0], "pwd", ft_strlen("pwd")) == 0 
            && ft_strlen("pwd") == ft_strlen(&cmd[i]->cmd[0]))//Caso pwd 
            printf_pwd(env);
        else if (ft_strncmp(&cmd[i]->cmd[0], "exit", ft_strlen("exit")) == 0 
            && ft_strlen("exit") == ft_strlen(&cmd[i]->cmd[0]))//Caso exit
            exit(0);
        i++;
    }
}