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
    printf("%s\n", path);
    return path;
}
//{
  //  while ()

//}

void execute_cmd(t_command **cmd, t_env *env)
{
    char *path = find_path(cmd, env);
    char **paths = ft_split(path, ':');
    int i = 0;

    while (paths[i])
    {
        char *full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(full_path, (*cmd)->cmd);
        if (access(full_path, F_OK) == 0)
        {
            execve(full_path, (*cmd)->args, NULL);
            free(full_path);
            break;
        }
        free(full_path);
        i++;
    }

    ft_putstr_fd("command not found", 2);
    ft_putstr_fd((*cmd)->cmd, 2);
    ft_putstr_fd("\n", 2);
    exit(127);
}