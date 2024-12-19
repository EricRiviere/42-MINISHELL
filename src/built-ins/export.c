#include "minishell.h"

static int is_valid_key(char c)
{
    return (ft_isalpha(c) || c == '_');
}

void export_new_env(t_command **cmd, t_env *env)
{
    int i = 0;
    int start;
    int j = 0;
    int end = 0;
    char *key;
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
            printf("soy key------------------>*%s*\n", key);
        }
        i++;
         
    } 
    
}