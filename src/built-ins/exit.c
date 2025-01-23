/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:18:49 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:19:27 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_command *cmd)
{
	int	exit_code;

	printf("exit\n");
	if (!cmd->args[0])
		exit(0);
	if (cmd->args[1])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return ;
	}
	if (!is_numeric_arg(cmd->args[0]))
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);
	}
	exit_code = ft_atoi(cmd->args[0]);
	exit_code %= 256;
	if (exit_code < 0)
		exit_code += 256;
	exit(exit_code);
}
