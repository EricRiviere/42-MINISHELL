/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:29:30 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 12:29:31 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_fd(t_command *cmd, const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (perror(file), -1);
	close_fd_if_open(&cmd->fd_in);
	cmd->fd_in = fd;
	return (0);
}

int	process_output_fd(t_command *cmd, const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror(file), -1);
	close_fd_if_open(&cmd->fd_out);
	cmd->fd_out = fd;
	return (0);
}

int	process_append_fd(t_command *cmd, const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror(file), -1);
	close_fd_if_open(&cmd->fd_out);
	cmd->fd_out = fd;
	return (0);
}

void	free_failed_commands(t_command **cmd_list, int cmd_index)
{
	while (--cmd_index >= 0)
		free_cmd_list(&cmd_list[cmd_index]);
	free(cmd_list);
}

t_command	**commands(t_token *tkn_lst)
{
	t_token		*curr_tkn;
	t_command	**cmd_list;
	t_command	*cmd;
	int			cmd_index;

	curr_tkn = tkn_lst;
	cmd_list = malloc(sizeof(t_command *) * 1000);
	if (!cmd_list)
		return (NULL);
	cmd_index = 0;
	while (curr_tkn)
	{
		cmd = init_command();
		if (!cmd)
			return (free_failed_commands(cmd_list, cmd_index), NULL);
		process_tokens(cmd, cmd->redirections, &curr_tkn);
		cmd_list[cmd_index++] = cmd;
		if (curr_tkn && curr_tkn->type == 2
			&& ft_strncmp(curr_tkn->value, "|", 1) == 0)
			curr_tkn = curr_tkn->next;
	}
	cmd_list[cmd_index] = NULL;
	return (cmd_list);
}
