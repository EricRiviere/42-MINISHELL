#include "minishell.h"
static int	check_flag(t_command **cmd)
{
	int	i;
	i = 0;
	if (cmd[0]->args[0] == NULL)
		return (0);
	else if (ft_strncmp(cmd[0]->args[0], "-n", ft_strlen("-n")) == 0 
        && ft_strlen("-n") == ft_strlen(cmd[0]->args[0]))
    {
        while (ft_strncmp(cmd[0]->args[i], "-n", ft_strlen("-n")) == 0 
        && ft_strlen("-n") == ft_strlen(cmd[0]->args[i]))
            i++;
		return (i);
    }
    else
        return (0);
}
void get_echo(t_command **cmd)
{
    int i = 0;
    int status = check_flag(cmd);
    while (cmd[0]->args[i])
    {
        if (!status)
        {
            while (cmd[0]->args[i] != NULL)
            {
                printf("%s", (cmd[0]->args[i]));
                if (cmd[0]->args[i + 1])
                    printf(" ");
                i++;
            }
            printf("\n");         
        }
        else
        {
            i += status;
            while (cmd[0]->args[i] != NULL)
            {
                printf("%s", (cmd[0]->args[i]));
                if (cmd[0]->args[i + 1])
                    printf(" ");
                i++;
            }
        }
    }
}