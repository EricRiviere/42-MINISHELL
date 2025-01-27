/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:29:40 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 12:51:06 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*allocate_command_resources(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redirections = malloc(sizeof(t_redir));
	if (!cmd->redirections)
		return (free(cmd), NULL);
	cmd->args = malloc(sizeof(char *) * 10000);
	if (!cmd->args)
		return (free(cmd->redirections), free(cmd), NULL);
	cmd->redirections->operator = malloc(sizeof(char *) * 500);
	cmd->redirections->file = malloc(sizeof(char *) * 500);
	if (!cmd->redirections->operator || !cmd->redirections->file)
		return (free_command_resources(cmd), NULL);
	return (cmd);
}

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = allocate_command_resources();
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->status = 0;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	return (cmd);
}

void	add_redirection(t_redir *redir, t_token *curr_tkn, int *op_index)
{
	redir->operator[*op_index] = ft_strdup(curr_tkn->value);
	if (curr_tkn->hd_fd != 1)
		redir->file[*op_index] = ft_itoa(curr_tkn->hd_fd);
	else
		redir->file[*op_index] = ft_strdup(curr_tkn->next->value);
	(*op_index)++;
}

void	close_fd_if_open(int *fd)
{
	if (*fd != -1)
		close(*fd);
}

int	process_redirections(t_command *cmd, t_redir *redir)
{
	int	i;

	i = 0;
	while (redir->operator[i])
	{
		if (ft_strncmp(redir->operator[i], ">>", 3) == 0)
		{
			if (process_append_fd(cmd, redir->file[i]) == -1)
				return (-1);
		}
		else if (ft_strncmp(redir->operator[i], "<<", 3) == 0)
			cmd->fd_in = ft_atoi(redir->file[i]);
		else if (ft_strncmp(redir->operator[i], "<", 2) == 0)
		{
			if (process_input_fd(cmd, redir->file[i]) == -1)
				return (-1);
		}
		else if (ft_strncmp(redir->operator[i], ">", 2) == 0)
		{
			if (process_output_fd(cmd, redir->file[i]) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
