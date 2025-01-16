#include "minishell.h"

void    free_env_list(t_env *env_list)
{
    t_env   *tmp;
    while (env_list)
    {
        tmp = env_list;
        env_list = env_list->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

void    add_env_variable(t_env **env_list, char *key, char *value)
{
    t_env   *new_node;
    
    if (!(new_node = malloc(sizeof(t_env))))
    {
        free_env_list(*env_list);
        perror("malloc error creating env node\n");
        return ;
    }
    if (!(new_node->key = ft_strdup(key)))
    {
        free(new_node);
        perror("malloc error for env key\n");
        return ;
    }
    if (!(new_node->value = ft_strdup(value)))
    {
        free(new_node->key);
        free(new_node);
        perror("malloc error for env value\n");
        return ;
    }
    new_node->next = *env_list;
    *env_list = new_node;
}

static void    double_free_char(char *s1, char *s2)
{
    if (!s1 || !s2)
        return ;
    free(s1);
    free(s2);
}

t_env *init_env_list(char **env)
{
    t_env *env_list;
    char *separator;
    char *key;
    char *value;

    env_list = NULL;
    while (*env)
    {
        separator = ft_strchr(*env, '=');
        if (separator)
        {
            key = ft_strndup(*env, separator - *env);
            value = ft_strdup(separator + 1);
            if (!key || !value)
            {
                perror("malloc error in environment variable duplication");
                double_free_char(key, value);
                free_env_list(env_list);
                return (NULL);
            }
            add_env_variable(&env_list, key, value);
            double_free_char(key, value);
        }
        env++;
    }
    return (env_list);
}


void    print_env_list(t_env *env_list)
{
    while(env_list)
    {
        printf("%s=%s\n", env_list->key, env_list->value);
        env_list = env_list->next;
    }
}

char    *get_env_value(char *str, t_env *env_list)
{
    while (env_list)
    {
        if (ft_strncmp(str, env_list->key, ft_strlen(str)) == 0 && ft_strlen(str) == ft_strlen(env_list->key))  
            return (env_list->value);
        env_list = env_list->next;
    }
    return (NULL);
}