#include "minishell.h"

static char *find_path(t_command **cmd, t_env *env)
{
    t_env *curr = env;
    char *path = NULL;

    while (curr)
    {
        if (ft_strncmp(curr->key, "PATH", 4) == 0)
        {
            path = curr->value;
            break;
        }
        curr = curr->next;
    }

    if (!path)
    {
        ft_putstr_fd("minishell: PATH not set\n", 2);
        return NULL;
    }
    return path;
}

int arr_len(char **cmds)
{
    int i = 0;
    while (cmds[i])
        i++;
    return (i);
}

char **new_args(t_command **cmd)
{
    int len;
    int i = 1;
    int j = 0;
    char **new_arr;

    if ((*cmd)->cmd == NULL)
        return (NULL);
    len = arr_len((*cmd)->args);
    new_arr = malloc(sizeof(char *) * (len + 2));
    if (new_arr == NULL)
        return (NULL);
    new_arr[0] = ft_strdup((*cmd)->cmd);
    while ((*cmd)->args[j])
        new_arr[i++] = ft_strdup((*cmd)->args[j++]);
    new_arr[i] = NULL;
    return (new_arr);
}
void execute_cmd(t_command **cmd, t_env *env)
{
    int id;
    int status;
    char **new_arr;
    int i;

    id = 0;
    status = 0;
    if ((*cmd)->cmd == NULL)
        return ;
    new_arr = new_args(cmd);
    id = fork();
    if (id == -1)
    {
        perror("fork");
        return ;
    }
    if (id == 0)
    {
        if ((*cmd)->cmd[0] == '/')
        {
            if (access((*cmd)->cmd, F_OK | X_OK | R_OK) == 0)
                execve((*cmd)->cmd, new_arr, NULL);
            else
            {
                ft_putstr_fd("command not found : ", 2);
                ft_putstr_fd((*cmd)->cmd, 2);
                ft_putstr_fd("\n", 2);
            }
            exit(127) ;
        }
        char *path = find_path(cmd, env);
        char **paths = ft_split(path, ':');
        i = 0;
        while (paths[i])
        {
            char *full_path = ft_strjoin(paths[i], "/");
            full_path = ft_strjoin(full_path, (*cmd)->cmd);
            if (access(full_path, F_OK | X_OK | R_OK) == 0)
            {
                execve(full_path, new_arr, NULL);
                free(full_path);
                break;
            }
            free(full_path);
            i++;
        }
        ft_putstr_fd("command not found : ", 2);
        ft_putstr_fd((*cmd)->cmd, 2);
        ft_putstr_fd("\n", 2);
        exit(127);
    }
    else
        waitpid(id, &status, 0);
}