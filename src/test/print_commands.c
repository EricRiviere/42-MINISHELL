/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:44:28 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:46:04 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_args(t_command *cmd)
{
	int	j;

	printf("  Args:");
	if (cmd->args && cmd->args[0])
	{
		j = 0;
		while (cmd->args[j])
		{
			printf(" *%s*", cmd->args[j]);
			j++;
		}
	}
	else
		printf(" (none)");
	printf("\n");
}

static void	print_redirections(t_command *cmd)
{
	int	j;

	printf("  Redirections:\n");
	if (cmd->redirections->operator)
	{
		j = 0;
		while (cmd->redirections->operator[j])
		{
			printf("    Operator: %s, File: %s\n",
				cmd->redirections->operator[j],
				cmd->redirections->file[j]);
			j++;
		}
	}
	else
		printf("    (none)\n");
}

static void	print_cmd_details(t_command *cmd, int index)
{
	printf("Command #%d:\n", index + 1);
	if (cmd->cmd)
		printf("  Cmd: *%s*\n", cmd->cmd);
	else
		printf("  Cmd: *(none)*\n");
	print_args(cmd);
	print_redirections(cmd);
	printf("  File descriptors:\n");
	printf("    Input fd: %d\n", cmd->fd_in);
	printf("    Output fd: %d\n", cmd->fd_out);
	printf("\n");
}

void	print_commands(char *line, t_command **cmd_list)
{
	int	i;

	printf("\n=== Input Line ===\n%s\n", line);
	printf("=== Command List ===\n");
	if (!cmd_list)
		return ((void)printf("No commands to display.\n"));
	i = 0;
	while (cmd_list[i])
	{
		print_cmd_details(cmd_list[i], i);
		i++;
	}
	printf("=====================\n");
}
