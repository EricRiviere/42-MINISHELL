#include "minishell.h"

void    print_declared_env(t_env **env_list)
{
    t_env *curr_var;

    curr_var = *env_list;
    while(curr_var)
    {
        if (ft_strncmp(curr_var->key, "?", -1) == 0)
            curr_var = curr_var->next;
        else if (curr_var->value == NULL)
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
        if (ft_strncmp(curr_var->key, "?", -1) == 0)
            curr_var = curr_var->next;
        else if (curr_var->value != NULL && curr_var->value[0] != '\0')
            printf("%s=%s\n", curr_var->key, curr_var->value);
        curr_var = curr_var->next;
    }
}

int is_builtin(t_command *cmd)
{
    if (ft_strncmp(&cmd->cmd[0], "env", -1) == 0)
        return (1);
    else if (ft_strncmp(&cmd->cmd[0], "cd", -1) == 0)
        return (2);
    else if (ft_strncmp(&cmd->cmd[0], "export", -1) == 0)
        return (3);
    else if (ft_strncmp(&cmd->cmd[0], "unset", -1) == 0)
        return (4);
    else if (ft_strncmp(&cmd->cmd[0], "pwd", -1) == 0)
        return (5);
    else if (ft_strncmp(&cmd->cmd[0], "exit", -1) == 0)
        return (6);
    else if (ft_strncmp(&cmd->cmd[0], "echo", -1) == 0)
        return (7);
    else
        return (0);
}

void manage_cd(t_command *cmd, t_env **env)
{
    if (!cmd->args || !cmd->args[0])
        change_dir(cmd, env);
    else if (cmd->args[1])
        ft_putstr_fd("minishell: cd: too many arguments", 2);
    else
        change_dir(cmd, env);
}

void manage_export(t_command *cmd, t_env **env)
{
    if (!cmd->args[0])
        print_declared_env(env); // Impresion con export
    else
        export_new_var(&cmd, env);
}

void    manage_builtins(t_command *cmd, t_env **env)
{
    if (&cmd->cmd[0] == NULL)
        return ;
    else if (is_builtin(cmd) == 1)//ENV
        print_enviroment(env);
    else if (is_builtin(cmd) == 2)//CD
        manage_cd(cmd, env);
    else if (is_builtin(cmd) == 3)//EXPORT
        manage_export(cmd, env);
    else if (is_builtin(cmd) == 4)//UNSET
        delete_env_var(&cmd, env);
    else if (is_builtin(cmd) == 5)//PWD
        printf_pwd(env);
    else if (is_builtin(cmd) == 6)//EXIT
        exit(0);
    else if (is_builtin(cmd) == 7)//ECHO
        get_echo(cmd);
}