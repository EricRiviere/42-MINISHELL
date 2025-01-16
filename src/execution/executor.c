#include "minishell.h"

static char *find_path(t_env *env)
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

char **env_arr(t_env *env_list)
{
    int list_len = 0;
    t_env *temp = env_list;
    while(temp)
    {
        temp = temp->next;
        list_len++;
    }
    char **arr = malloc(sizeof(char *) * (list_len + 1));
    if (!arr)
    {
        perror("malloc error");
        return (NULL);
    }
    int i = 0;
    while (env_list)
    {
        char *temp = ft_strjoin(env_list->key, "=");
        if (!temp)
        {
            perror("malloc error");
            free(arr);
            return (NULL);
        }
        char *var = ft_strjoin(temp, env_list->value);
        free(temp);
        if (!var)
        {
            perror("malloc error");
            free(arr);
            return (NULL);
        }
        arr[i] = var;
        env_list = env_list->next;
        i++;
    }
    arr[i] = NULL;
    return (arr);
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

void free_array(char **arr)
{
    int i;
    
    i = 0;
    while (arr && arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int get_cmd_num(t_command **cmd)
{
    int num;
    int idx;
    num = 0;
    idx = 0;
    if (cmd == NULL)
        return (num);
    if ((*cmd)->cmd == NULL)
        return (num);
    while (cmd[idx] != NULL)
    {
        num++;
        idx++;
    }
    return (num);
}

void execute_cmd(t_command **cmd, t_env **env)
{
    char **new_arr;
    int i;
    int cmd_num = get_cmd_num(cmd);

    if (cmd == NULL)
        return ;
    if ((*cmd)->cmd == NULL)
        return ;
    if (cmd_num == 1 && is_builtin(*cmd) != 0)
    {
        manage_builtins(*cmd, env);
        if ((*cmd)->fd_out != -1)
            close((*cmd)->fd_out);
    }
    else
    {
        new_arr = new_args(cmd);
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
            free_array(new_arr);
            exit(127);
        }
        else if ((*cmd)->cmd[0] == '.')
        {
            char **arr = env_arr(*env);
            if (access((*cmd)->cmd, F_OK | X_OK | R_OK) == 0)
                execve((*cmd)->cmd, new_arr, arr);
            else
            {
                ft_putstr_fd("command not found : ", 2);
                ft_putstr_fd((*cmd)->cmd, 2);
                ft_putstr_fd("\n", 2);
            }
            free_array(arr);
            free_array(new_arr);
            exit(127);
        }
        char *path = find_path(*env);
        char **paths = ft_split(path, ':');
        i = 0;
        while (paths[i])
        {
            char *tmp =  ft_strjoin(paths[i], "/");
            char *full_path = ft_strjoin(tmp, (*cmd)->cmd);
            if  (tmp)
                free(tmp);
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
        free_array(new_arr);
        exit(127);
    }
    int term_out = 0;
    int term_in = 0;
    if (dup2(term_out, STDOUT_FILENO) == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
    if (dup2(term_in, STDIN_FILENO) == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
}