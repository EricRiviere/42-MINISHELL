#include "minishell.h"

void    printf_pwd(t_env **env)
{
    t_env *env_lst;
    char   *path;

    env_lst = *env;
    path = NULL;
    while (env_lst)
    {
        if(ft_strncmp(env_lst->key, "PWD", 3) && ft_strlen(env_lst->key) == 3)
            printf("%s\n", env_lst->value);
        env_lst = env_lst->next;
    }
    if (!env_lst)
    {
        path = getcwd(NULL, 0);
        printf("%s\n", path);
        free(path);
    }
}