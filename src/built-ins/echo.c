#include "minishell.h"

static int	check_flag(t_command **cmd)
{
	int	i;

	i = 0;
	// Verificamos que cmd[0] y cmd[0]->args existan y no sean NULL
	if (!cmd || !cmd[0] || !cmd[0]->args || cmd[0]->args[0] == NULL)
		return (0);

	// Verificamos si el primer argumento es "-n"
	if (ft_strncmp(cmd[0]->args[0], "-n", ft_strlen("-n")) == 0 
		&& ft_strlen("-n") == ft_strlen(cmd[0]->args[0]))
	{
		// Comprobamos si hay múltiples "-n" consecutivos
		while (cmd[0]->args[i] && ft_strncmp(cmd[0]->args[i], "-n", ft_strlen("-n")) == 0 
			&& ft_strlen("-n") == ft_strlen(cmd[0]->args[i]))
			i++;
		return (i);
	}
	return (0);
}

void get_echo(t_command **cmd)
{
	int i;
	int status;

	// Verificamos que cmd y cmd[0] existen para evitar segfaults
	if (!cmd || !cmd[0] || !cmd[0]->args)
		return;

	i = 0;
	status = check_flag(cmd);

	if (!cmd[0]->args[i])
		printf("\n");
	// Iteramos a través de los argumentos
	while (cmd[0]->args[i])
	{
		if (!status) // Sin flag "-n"
		{
			while (cmd[0]->args[i] != NULL)
			{
				printf("%s", (cmd[0]->args[i]));
				if (cmd[0]->args[i + 1])
					printf(" ");
				i++;
			}
			printf("\n");
			return;
		}
		else // Con flag "-n"
		{
			i += status;
			while (cmd[0]->args[i] != NULL)
			{
				if (cmd[0]->fd_out != -1)
					ft_putstr_fd((cmd[0]->args[i]), cmd[0]->fd_out);
				else
					printf("%s", (cmd[0]->args[i]));
				if (cmd[0]->args[i + 1])
				{
					if (cmd[0]->fd_out != -1)
						ft_putstr_fd(" ", cmd[0]->fd_out);
					else
						printf(" ");
				}
				i++;
			}
			return; // No imprimimos el salto de línea
		}
	}
}
