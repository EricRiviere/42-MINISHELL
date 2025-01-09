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
    cmd->fd_in = -1;
    cmd->fd_out = -1;
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
void manage_word_quote_cmd(t_command *cmd, t_token **curr_tkn, int *arg_index)
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

void close_fd_if_open(int *fd)
{
    if (*fd != -1)
        close(*fd);
}

int process_input_fd(t_command *cmd, const char *file)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd == -1)
        return (perror(file), -1); // Error al abrir archivo
    close_fd_if_open(&cmd->fd_in); // Cerramos el fd_in anterior
    cmd->fd_in = fd;
    if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int process_output_fd(t_command *cmd, const char *file)
{
    int fd;

    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (perror(file), -1); // Error al abrir archivo
    close_fd_if_open(&cmd->fd_out); // Cerramos el fd_out anterior
    cmd->fd_out = fd;
    if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int process_append_fd(t_command *cmd, const char *file)
{
    int fd;

    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
        return (perror(file), -1); // Error al abrir archivo
    close_fd_if_open(&cmd->fd_out); // Cerramos el fd_out anterior
    cmd->fd_out = fd;
    if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int process_redirections(t_command *cmd, t_redir *redir)
{
    int i;

    i = 0;
    while (redir->operator[i])
    {
        if (ft_strncmp(redir->operator[i], ">>", 3) == 0)
        {
            if (process_append_fd(cmd, redir->file[i]) == -1)
                return -1;
        }
        else if (ft_strncmp(redir->operator[i], "<", 2) == 0)
        {
            if (process_input_fd(cmd, redir->file[i]) == -1)
                return -1;
        }
        else if (ft_strncmp(redir->operator[i], ">", 2) == 0)
        {
            if (process_output_fd(cmd, redir->file[i]) == -1)
                return -1;
        }
        // else
        // {
        //     cmd->fd_in = STDIN_FILENO;
        // }
        i++;
    }
    return 0;
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
            manage_word_quote_cmd(cmd, curr_tkn, &arg_index);
        else
            *curr_tkn = (*curr_tkn)->next;
    }
    cmd->args[arg_index] = NULL; // Finalizar listas de argumentos y redirecciones
    redir->operator[op_index] = NULL;
    redir->file[op_index] = NULL;
    if (process_redirections(cmd, redir) == -1)// Procesar redirección y cerrar fds abiertos si es necesario
    {
        cmd->redir_error = 1;
        return;
    }
    cmd->redir_error = 0;
}

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

void free_single_cmd(t_command *cmd)
{
    int j;

    if (!cmd)
        return;
    j = 0;
    while (cmd->args && cmd->args[j])
        free(cmd->args[j++]);
    free(cmd->args);
    if (cmd->redirections)
        free_redir(cmd->redirections);
    if (cmd->fd_in != -1)
        close(cmd->fd_in);
    if (cmd->fd_out != -1)
        close(cmd->fd_out);
    free(cmd->cmd);
    free(cmd);
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
        if (cmd->redir_error)
        {
            free_single_cmd(cmd);
            if (cmd_list)
                free(cmd_list);
            return (NULL);
        }
        cmd_list[cmd_index++] = cmd;
        if (curr_tkn && curr_tkn->type == 2 && ft_strncmp(curr_tkn->value, "|", 1) == 0)
            curr_tkn = curr_tkn->next;
    }
    cmd_list[cmd_index] = NULL;
    return (cmd_list);
}

void free_cmd_list(t_command **cmd_list)
{
    int i;
    int j;
    t_command *cmd;

    if (!cmd_list)
        return;
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
        if (cmd->fd_in != -1) // Cerrar fd_in personalizado
            close(cmd->fd_in);
        if (cmd->fd_out != -1) // Cerrar fd_out personalizado
            close(cmd->fd_out);
        free(cmd->cmd);// Liberar comando principal
        free(cmd);// Liberar el comando completo
        i++;
    }
    free(cmd_list);// Liberar lista de comandos
}
