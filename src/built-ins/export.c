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
        if (ft_strncmp(key, curr_node->key, ft_strlen(key)) == 0 && ft_strlen(key) == ft_strlen(curr_node->key))
            return (curr_node);
        else
            curr_node = curr_node->next;
    }
    return (NULL);
}

void    cu_env_var(t_env **env, char *key, char *value)
{
    t_env *curr_node;

    curr_node = get_var(env, key);
    if (curr_node)
    {
        if (curr_node->value)
            free(curr_node->value);
        curr_node->value = ft_strdup(value);
    }
    else
        add_env_variable(env, key, value);
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
                value = ft_strdup(&(*cmd)->args[i][j + 1]);
                cu_env_var(env, key, value);
            }
            else
            {
                if (!(curr_node = get_var(env, key)))
                    cu_env_var(env, key, "");
            }
            free(key);
            if ((*cmd)->args[i][j] == '=')
                free(value);
        }
        i++; 
    } 
}