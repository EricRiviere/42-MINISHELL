#include "minishell.h"

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

        printf("  Cmd: *%s*\n", curr_cmd->cmd ? curr_cmd->cmd : "(none)");

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
        printf("  File descriptors:\n");
        printf("    Input fd: %d\n", curr_cmd->fd_in);
        printf("    Output fd: %d\n", curr_cmd->fd_out);
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