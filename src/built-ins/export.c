#include "minishell.h"

static int is_valid_key(char c)
{
    return (ft_isalpha(c) || c == '_');
}

void export_new_env(t_command **cmd, t_env **env)
{
    int i = 0;
    int start;
    int j = 0;
    int end = 0;
    char *key;
    char *value;
    //if ((*cmd)->args == NULL)
        //print_env con todas las variables including NULL values;
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
                value = ft_strdup(&(*cmd)->args[i][j + 1]);
            else
                value = ft_strdup("");
            add_env_variable(env, key, value);
            free(key);
            free(value);
        }
        i++; 
    } 
    
}