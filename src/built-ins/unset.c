#include "minishell.h"

static void free_env_var(t_env *env)
{
    free(env->key);
    free(env->value);
    free(env);
}

void unset_env_var(t_command **cmd, t_env **env)
{
    t_env *new_var = *env;
    t_env *prev = NULL;
    int i = 0;
    while ((*cmd)->args[i])
    {
        while (new_var != NULL)
        {
            if (ft_strncmp((*cmd)->args[i], (new_var)->key, ft_strlen((new_var)->key)) == 0)
            {
                if (prev == NULL)
                    *env = new_var->next;
                else
                    prev->next = new_var->next;
                free_env_var(new_var);
                return ;
            }
            prev = new_var;
            new_var = new_var->next;
        }
        i++;
    }
}