#include "minishell.h"

// abre fds infinitos pero no se cierra cuando devuelve su nombre
// faltaria un strncmp que chequee si line == heredoc name y se cierre
// hacer un dup para redirigir el output en caso de que se rediriga el heredoc a outfile
static void process_heredoc(t_token *heredoc_token)
{
    int fd;
    char *line;
    fd = open("/tmp/heredoc_pipe", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("Error opening heredoc", 2);
        return ;
    }
    while (1)
    {
        line = readline(">");
        if (ft_strncmp(line, heredoc_token->next->value, ft_strlen(heredoc_token->next->value)) == 0)
        {
            free(line);
            break;
        }
        if (line == NULL)
            break ;
        write(fd, line, ft_strlen(line));
        write(fd, "\n" ,1);
        free(line);
    }
    close(fd);
}

//------------------------------------------- PRINT COMMANDS
void print_commands(char *line, t_command **cmd_list)
{
    int i = 0;

    printf("\n=== Input Line ===\n");
    printf("%s\n", line);
    printf("=== Command List ===\n");

    if (!cmd_list)
    {
        printf("No commands to display.\n");
        return;
    }

    while (cmd_list[i])
    {
        t_command *curr_cmd = cmd_list[i];
        printf("Command #%d:\n", i + 1);

        printf("  Cmd: %s\n", curr_cmd->cmd ? curr_cmd->cmd : "(none)");

        printf("  Args:");
        if (curr_cmd->args && curr_cmd->args[0])
        {
            for (int j = 0; curr_cmd->args[j]; j++)
                printf(" *%s*", curr_cmd->args[j]);
        }
        else
        {
            printf(" (none)");
        }
        printf("\n");

        printf("  Redirections:\n");
        if (curr_cmd->redirections->operator)
        {
            for (int j = 0; curr_cmd->redirections->operator[j]; j++)
            {
                printf("    Operator: %s, File: %s\n",
                       curr_cmd->redirections->operator[j],
                       curr_cmd->redirections->file[j]);
            }
        }
        else
        {
            printf("    (none)\n");
        }

        printf("\n");
        i++;
    }

    printf("=====================\n");
}

//-------------------------------------------
//-------------------------------------------PRINT TOKENS
void print_tokens(char *line, t_token *tkn_lst)
{
    t_token *curr_tkn = tkn_lst;

    printf("\n=== Input Line ===\n");
    printf("%s\n", line);
    printf("=== Tokens ===\n");

    while (curr_tkn)
    {
        printf("Token value: *%s*\n", curr_tkn->value);
        printf("  - is quote? %d\n", curr_tkn->is_quote);
        printf("  - expands? %d\n", curr_tkn->expand);
        printf("  - Number of spaces: %d\n", curr_tkn->has_space);
        printf("  - Type: %u\n", curr_tkn->type);
        curr_tkn = curr_tkn->next;
    }

    printf("===================\n");
}
//-------------------------------------------
int main(int argc, char **argv, char **env)
{
    char    *line;
    t_token *tkn_lst;
    t_token *curr_tkn;
    t_env   *env_lst;

    (void)argv;
    if (argc != 1)
    {
        printf("Wrong number of arguments\n");
        return (1);
    }
    env_lst = init_env_list(env);
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (line)
        {
            if (ft_strncmp(line, "env", 3) == 0)
                print_env_list(env_lst);
            add_history(line);
            tkn_lst = tokenize(line);
            if (tkn_lst && !syntax_check(tkn_lst))
            {
                free_tkn_lst(tkn_lst);
                free(line);
                continue;
            }
            if (tkn_lst)
            {
                curr_tkn = tkn_lst;
                while (curr_tkn)
                {
                    if (ft_strncmp(curr_tkn->value, "<<", 2) == 0)
                    {
                        process_heredoc(curr_tkn);
                    }
                    else
                    {
                        expand_variables(curr_tkn, env_lst);
                    }
                    curr_tkn = curr_tkn->next;
                }
                curr_tkn = tkn_lst;
            }
            if (tkn_lst)
            {
                preprocess_tokens(&tkn_lst);
                t_command **cmd_list = commands(tkn_lst);
                execute_cmd(&cmd_list, env_lst);
                print_commands(line, cmd_list);
                print_tokens(line, tkn_lst);
                free_cmd_list(cmd_list);
            }
            free_tkn_lst(tkn_lst);
        }
        free(line);
    }
    free_env_list(env_lst);
    return (0);
}