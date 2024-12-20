#include "minishell.h"

void preprocess_tokens(t_token **tkn_lst)
{
    t_token *curr;
    t_token *prev;
    char *temp;
    
    curr = *tkn_lst;
    while (curr)
    {
        if ((curr->type == WORD || curr->type == QUOTED) && curr->has_space == 0)
        {
            prev = curr->prev;
            if (prev && (prev->type == WORD || prev->type == QUOTED)) // Validar tipo del token anterior
            {
                temp = prev->value;
                prev->value = ft_strjoin(prev->value, curr->value); // Concatenar al token anterior
                free(temp);
                prev->next = curr->next;// Eliminar el token actual después de la concatenación
                if (curr->next)
                    curr->next->prev = prev;
                free(curr->value);
                free(curr);
                curr = prev->next;
                continue;
            }
        }
        curr = curr->next;
    }
}

// Función para liberar recursos en caso de error
void free_command_resources(t_command *cmd, char **args, char **operator, char **file)
{
    if (cmd)
        free(cmd);
    if (args)
        free(args);
    if (operator)
        free(operator);
    if (file)
        free(file);
}

t_command *init_command()// Función para inicializar un comando
{
    t_command *cmd;
    t_redir *redir;
    char **args;
    char **operator;
    char **file;

    cmd = malloc(sizeof(t_command));
    redir = malloc(sizeof(t_redir));
    args = malloc(sizeof(char *) * 100);
    operator = malloc(sizeof(char *) * 50);
    file = malloc(sizeof(char *) * 50);
    if (!cmd || !redir || !args || !operator || !file)
    {
        free_command_resources(cmd, args, operator, file);
        free(redir);
        return NULL;
    }
    cmd->cmd = NULL;
    cmd->args = args;
    cmd->redirections = redir;
    redir->operator = operator;
    redir->file = file;
    return (cmd);
}

// Función para agregar una redirección
void add_redirection(t_redir *redir, t_token *curr_tkn, int *op_index)
{
    redir->operator[*op_index] = ft_strdup(curr_tkn->value);
    redir->file[*op_index] = ft_strdup(curr_tkn->next->value);
    (*op_index)++;
}

// Funcion manejo redirecciones
void manage_redirection_cmd(t_token **curr_tkn, t_redir *redir, int *op_index)
{
    if ((*curr_tkn)->value && (*curr_tkn)->next->value)
        add_redirection(redir, *curr_tkn, op_index);
    *curr_tkn = (*curr_tkn)->next->next;
}

// Funcion manejo words y quotes
void manage_word_quote_cmd(t_command *cmd, t_token **curr_tkn, t_redir *redir, int *arg_index)
{
    if ((*curr_tkn)->value)
    {
        if (!cmd->cmd)
            cmd->cmd = ft_strdup((*curr_tkn)->value); // Asignar como comando
        else
            cmd->args[(*arg_index)++] = ft_strdup((*curr_tkn)->value); // Agregar argumento
    }
    *curr_tkn = (*curr_tkn)->next;
}

// Función para procesar tokens y llenar un comando
void process_tokens(t_command *cmd, t_redir *redir, t_token **curr_tkn)
{
    int arg_index;
    int op_index;

    arg_index = 0;
    op_index = 0;
    while (*curr_tkn && !((*curr_tkn)->type == 2 && ft_strncmp((*curr_tkn)->value, "|", 1) == 0))
    {
        if ((*curr_tkn)->type == 2 && (*curr_tkn)->next) // Redirección
            manage_redirection_cmd(curr_tkn, redir, &op_index);
        else if ((*curr_tkn)->type == 0 || (*curr_tkn)->type == 1) // Word o Quote
            manage_word_quote_cmd(cmd, curr_tkn, redir, &arg_index);
        else
            *curr_tkn = (*curr_tkn)->next;
    }
    cmd->args[arg_index] = NULL;// Finalizar listas
    redir->operator[op_index] = NULL;
    redir->file[op_index] = NULL;
}

t_command   **commands(t_token *tkn_lst)//Funcion principal
{
    t_token *curr_tkn;
    t_command **cmd_list;
    t_command *cmd;
    int cmd_index;

    curr_tkn = tkn_lst;
    if (!(cmd_list = malloc(sizeof(t_command *) * 100)))
        return (NULL);
    cmd_index = 0;
    while (curr_tkn)
    {
        if (!(cmd = init_command()))
        {
            while (--cmd_index >= 0)
                free_cmd_list(&cmd_list[cmd_index]);
            free(cmd_list);
            return (NULL);
        }
        process_tokens(cmd, cmd->redirections, &curr_tkn);
        cmd_list[cmd_index++] = cmd;
        if (curr_tkn && curr_tkn->type == 2 && ft_strncmp(curr_tkn->value, "|", 1) == 0)
            curr_tkn = curr_tkn->next;
    }
    cmd_list[cmd_index] = NULL;
    return (cmd_list);
}

// t_command **commands(t_token *tkn_lst)
// {
//     t_token *curr_tkn = tkn_lst;
//     t_command **cmd_list = malloc(sizeof(t_command *) * 100); // Máximo 100 comandos
//     int cmd_index = 0;

//     if (!cmd_list)
//         return NULL; // Manejar error de asignación

//     while (curr_tkn)
//     {
//         t_command *cmd = malloc(sizeof(t_command));
//         t_redir *redir = malloc(sizeof(t_redir));
//         char **args = malloc(sizeof(char *) * 100); // Máximo 100 argumentos
//         char **operator = malloc(sizeof(char *) * 50); // Máximo 50 redirecciones
//         char **file = malloc(sizeof(char *) * 50);
//         int arg_index = 0, op_index = 0;

//         if (!cmd || !redir || !args || !operator || !file)
//         {
//             // Liberar recursos en caso de error
//             free(cmd);
//             free(redir);
//             free(args);
//             free(operator);
//             free(file);
//             return NULL;
//         }

//         cmd->cmd = NULL;
//         cmd->args = args;
//         cmd->redirections = redir;

//         while (curr_tkn && !(curr_tkn->type == 2 && ft_strncmp(curr_tkn->value, "|", 1) == 0))
//         {
//             if (curr_tkn->type == 2 && curr_tkn->next) // Redirección
//             {
//                 if (curr_tkn->value && curr_tkn->next->value) // Validar que no sean NULL
//                 {
//                     operator[op_index] = ft_strdup(curr_tkn->value);
//                     file[op_index] = ft_strdup(curr_tkn->next->value);
//                     op_index++;
//                 }
//                 curr_tkn = curr_tkn->next->next;
//             }
//             else if (curr_tkn->type == 0 || curr_tkn->type == 1) // Word o Quote
//             {
//                 if (curr_tkn->value) // Validar que no sea NULL
//                 {
//                     if (!cmd->cmd) // Asignar el primer token como comando
//                         cmd->cmd = strdup(curr_tkn->value);
//                     else // Agregar como argumento
//                     {
//                         args[arg_index] = ft_strdup(curr_tkn->value);
//                         arg_index++;
//                     }
//                 }
//                 curr_tkn = curr_tkn->next;
//             }
//             else
//             {
//                 curr_tkn = curr_tkn->next;
//             }
//         }

//         // Finalizar listas
//         args[arg_index] = NULL;
//         operator[op_index] = NULL;
//         file[op_index] = NULL;
//         redir->operator = operator;
//         redir->file = file;

//         cmd_list[cmd_index] = cmd;
//         cmd_index++;

//         if (curr_tkn && curr_tkn->type == 2 && ft_strncmp(curr_tkn->value, "|", 1) == 0)
//             curr_tkn = curr_tkn->next;
//     }

//     cmd_list[cmd_index] = NULL;
//     return cmd_list;
// }

void    free_redir(t_redir *redir)
{
    int j;

    j = 0;
    while (redir->operator[j])
        free(redir->operator[j++]);
    free(redir->operator);
    j = 0;
    while (redir->file[j])
        free(redir->file[j++]);
    free(redir->file);
    free(redir); // Liberar la estructura redirección
}

void free_cmd_list(t_command **cmd_list)
{
    int i;
    int j;
    t_command *cmd;

    i = 0;
    while (cmd_list[i])
    {
        cmd = cmd_list[i];
        j = 0;
        while (cmd->args[j])// Liberar argumentos
            free(cmd->args[j++]);
        free(cmd->args);
        if (cmd->redirections)
            free_redir(cmd->redirections);// Liberar redirecciones
        free(cmd->cmd);// Liberar comando principal
        free(cmd);// Liberar el comando completo
        i++;
    }
    free(cmd_list);// Liberar lista de comandos
}
