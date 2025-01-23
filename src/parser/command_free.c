/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:29:58 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 12:29:59 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command_resources(t_command *cmd)
{
	if (cmd)
		free(cmd);
	if (cmd->args)
		free(cmd->args);
	if (cmd->redirections->operator)
		free(cmd->redirections->operator);
	if (cmd->redirections->file)
		free(cmd->redirections->file);
}

void	free_redir(t_redir *redir)
{
	int	j;

	j = 0;
	while (redir->operator[j])
		free(redir->operator[j++]);
	free(redir->operator);
	j = 0;
	while (redir->file[j])
		free(redir->file[j++]);
	free(redir->file);
	free(redir);
}

void	free_single_cmd(t_command *cmd)
{
	int	j;

	if (!cmd)
		return ;
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

void	free_cmd_list(t_command **cmd_list)
{
	int			i;
	int			j;
	t_command	*cmd;

	if (!cmd_list)
		return ;
	i = 0;
	while (cmd_list[i])
	{
		cmd = cmd_list[i];
		j = 0;
		while (cmd->args[j])
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
		i++;
	}
	free(cmd_list);
}
