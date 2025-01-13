#include "minishell.h"

static int is_valid_key(char c)
{
    return (ft_isalpha(c) || c == '_');
}

t_env *get_var(t_env **env, char *key)
{
    t_env   *curr_node;

    curr_node = *env;
    while (curr_node)
    {
        if (ft_strncmp(key, curr_node->key, -1) == 0)
            return (curr_node);
        else
            curr_node = curr_node->next;
    }
    return (NULL);
}

void add_env_variable_with_null(t_env **env_list, char *key)
{
    t_env   *new_node;
    
    new_node = malloc(sizeof(t_env));
    if (!new_node)
    {
        free_env_list(*env_list);
        perror("malloc error creating env node\n");
        return ;
    }
    new_node->key = ft_strdup(key);
    if (!new_node->key)
    {
        free(new_node);
        perror("malloc error for env key\n");
        return ;
    }
    new_node->value = NULL;
    new_node->next = *env_list;
    *env_list = new_node;
}


void    cu_env_var(t_env **env, char *key, char *value)
{
    t_env *curr_node;

    curr_node = get_var(env, key);
    if (curr_node)
    {
        if (curr_node->value)
            free(curr_node->value);
        if (value)
            curr_node->value = ft_strdup(value);
        else
            curr_node->value = NULL;
    }
    else
        if (value)
            add_env_variable(env, key, value);
        else
            add_env_variable_with_null(env, key);
}

void export_new_var(t_command **cmd, t_env **env)
{
    int i;
    int start;
    int j;
    int end;
    char *key;
    char *value;
    t_env *curr_node;

    i = 0;
    end = 0;
    while ((*cmd)->args[i])
    {
        j = 0;
        if (is_valid_key((*cmd)->args[i][j]))
        {
            start = j;
            while (((*cmd)->args[i][j] != '=') && (*cmd)->args[i][j] != '\0')
                j++;
            end = j;
            key = ft_strndup(&(*cmd)->args[i][start], (end - start));
            if ((*cmd)->args[i][j] == '=')
            {
                if ((*cmd)->args[i][j + 1] != '\0')
                {
                    value = ft_strdup(&(*cmd)->args[i][j + 1]);
                    cu_env_var(env, key, value);
                    free(value);
                }
                else
                    cu_env_var(env, key, "");
            }
            else
            {
                if (!(curr_node = get_var(env, key)))
                    cu_env_var(env, key, NULL);
            }
            free(key);
        }
        else
            ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
        i++; 
    } 
}