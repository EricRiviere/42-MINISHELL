/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:07:46 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:17:24 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_flag(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	if (!cmd || !cmd->args || cmd->args[0] == NULL)
		return (0);
	while (cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
	{
		j = 1;
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] != '\0')
			break ;
		i++;
	}
	return (i);
}

static void	echo_print_args(t_command *cmd, int i)
{
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
}

void	get_echo(t_command *cmd)
{
	int	flag;

	if (!cmd->args[0])
	{
		printf("\n");
		return ;
	}
	flag = check_flag(cmd);
	echo_print_args(cmd, flag);
	if (!flag)
		printf("\n");
}
