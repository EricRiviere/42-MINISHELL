#include "minishell.h"

void delete_env_var(t_command **cmd, t_env **env)
{
    int i;
    t_env *curr_node;
    t_env *temp;

    i = 0;
    while ((*cmd)->args[i])
    {
        curr_node = *env;
        temp = NULL;
        while (curr_node)
        {
            if (ft_strncmp((*cmd)->args[i], curr_node->key, ft_strlen((*cmd)->args[i])) == 0 &&
                ft_strlen((*cmd)->args[i]) == ft_strlen(curr_node->key))
            {
                if (temp == NULL)// Caso especial: eliminar la cabeza de la lista
                    *env = curr_node->next;
                else
                    temp->next = curr_node->next;
                free(curr_node->key);// Liberar el nodo y su contenido
                free(curr_node->value);
                free(curr_node);
                break; // Salir del bucle interno
            }
            temp = curr_node;
            curr_node = curr_node->next;
        }
        i++;
    }
}
