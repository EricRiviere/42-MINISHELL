#include "minishell.h"

static int check_flag(t_command *cmd)
{
    int i;

    i = 0;

    // Verificamos que cmd y cmd->args existan y no sean NULL
    if (!cmd || !cmd->args || cmd->args[0] == NULL)
        return (0);

    // Verificamos si el primer argumento es "-n"
    if (ft_strncmp(cmd->args[0], "-n", -1) == 0)
    {
        // Comprobamos si hay múltiples "-n" consecutivos
        while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", -1) == 0)
            i++;
        return (i);
    }
    return (0);
}

void get_echo(t_command *cmd)
{
    int i;
    int flag;

    i = 0;
    flag = check_flag(cmd);
    if (!cmd->args[i])
    {
        printf("\n");
        return;
    }
    // Procesamos los argumentos con o sin la flag "-n"
    if (flag) // Con flag "-n"
        i += flag;
    while (cmd->args[i])
    {
        if (cmd->fd_out != -1)
            ft_putstr_fd(cmd->args[i], 1);
        else
            printf("%s", cmd->args[i]);

        if (cmd->args[i + 1])
        {
            if (cmd->fd_out != -1)
                ft_putstr_fd(" ", 1);
            else
                printf(" ");
        }
        i++;
    }
    // Imprimimos el salto de línea solo si no hay "-n"
    if (!flag)
        printf("\n");
    return;
}

// static int	check_flag(t_command **cmd)
// {
// 	int	i;

// 	i = 0;
// 	// Verificamos que cmd[0] y cmd[0]->args existan y no sean NULL
// 	if (!cmd || !cmd[0] || !cmd[0]->args || cmd[0]->args[0] == NULL)
// 		return (0);

// 	// Verificamos si el primer argumento es "-n"
// 	if (ft_strncmp(cmd[0]->args[0], "-n", -1) == 0)
// 	{
// 		// Comprobamos si hay múltiples "-n" consecutivos
// 		while (cmd[0]->args[i] && ft_strncmp(cmd[0]->args[i], "-n", -1) == 0)
// 			i++;
// 		return (i);
// 	}
// 	return (0);
// }

// void get_echo(t_command **cmd)
// {
// 	int i;
// 	int status;

// 	// Verificamos que cmd y cmd[0] existen para evitar segfaults
// 	if (!cmd || !cmd[0] || !cmd[0]->args)
// 		return;

// 	i = 0;
// 	status = check_flag(cmd);

// 	if (!cmd[0]->args[i])
// 		printf("\n");
// 	// Iteramos a través de los argumentos
// 	while (cmd[0]->args[i])
// 	{
// 		if (!status) // Sin flag "-n"
// 		{
// 			while (cmd[0]->args[i] != NULL)
// 			{
// 				printf("%s", (cmd[0]->args[i]));
// 				if (cmd[0]->args[i + 1])
// 					printf(" ");
// 				i++;
// 			}
// 			printf("\n");
// 			return;
// 		}
// 		else // Con flag "-n"
// 		{
// 			i += status;
// 			while (cmd[0]->args[i] != NULL)
// 			{
// 				if (cmd[0]->fd_out != -1)
// 					ft_putstr_fd((cmd[0]->args[i]), 1);
// 				else
// 					printf("%s", (cmd[0]->args[i]));
// 				if (cmd[0]->args[i + 1])
// 				{
// 					if (cmd[0]->fd_out != -1)
// 						ft_putstr_fd(" ", 1);
// 					else
// 						printf(" ");
// 				}
// 				i++;
// 			}
// 			return; // No imprimimos el salto de línea
// 		}
// 	}
// }