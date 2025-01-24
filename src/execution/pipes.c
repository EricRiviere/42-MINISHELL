/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:02:45 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 20:00:49 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_unique_builtin(t_command *cmd)
{
	if (ft_strncmp(&cmd->cmd[0], "cd", -1) == 0)
		return (2);
	else if (ft_strncmp(&cmd->cmd[0], "export", -1) == 0)
		return (3);
	else if (ft_strncmp(&cmd->cmd[0], "unset", -1) == 0)
		return (4);
	else if (ft_strncmp(&cmd->cmd[0], "exit", -1) == 0)
		return (6);
	else
		return (0);
}

void	child_process(t_command **cmds, t_env **env, t_pipe_data *p_data, int i)
{
	signal(SIGINT, exit);
	signal(SIGQUIT, exit);
	if (p_data->prev_fd != -1)
	{
		dup2(p_data->prev_fd, STDIN_FILENO);
		close(p_data->prev_fd);
	}
	if (i < p_data->cmd_num - 1)
	{
		dup2(p_data->pipe_fd[1], STDOUT_FILENO);
		close(p_data->pipe_fd[1]);
		close(p_data->pipe_fd[0]);
	}
	if (cmds[i]->fd_in != -1)
	{
		dup2(cmds[i]->fd_in, STDIN_FILENO);
		close(cmds[i]->fd_in);
	}
	if (cmds[i]->fd_out != -1)
	{
		dup2(cmds[i]->fd_out, STDOUT_FILENO);
		close(cmds[i]->fd_out);
	}
	execute_cmd(&cmds[i], env);
	exit((*cmds)->status);
}

void	parent_process(t_pipe_data *p_data, t_command **cmds, t_env **env,
		int i)
{
	char	*val;

	if (p_data->prev_fd != -1)
		close(p_data->prev_fd);
	if (i < p_data->cmd_num - 1)
	{
		close(p_data->pipe_fd[1]);
		p_data->prev_fd = p_data->pipe_fd[0];
	}
	waitpid(p_data->pid, &(*cmds)->status, 0);
	if (WIFEXITED((*cmds)->status))
		(*cmds)->status = WEXITSTATUS((*cmds)->status);
	else
		handle_signaled(&(*cmds)->status, WTERMSIG((*cmds)->status));
	val = ft_itoa(get_status(1, (*cmds)->status));
	cu_env_var(env, "?", val);
	free (val);
}

int	check_single_builtin(t_command **cmds, t_env **env, t_pipe_data *p_data)
{
	if (p_data->cmd_num == 1 && is_unique_builtin(cmds[0]))
	{
		execute_cmd(&cmds[0], env);
		return (1);
	}
	return (0);
}

void	execute_pipes(t_command **cmds, t_env **env)
{
	t_pipe_data	p_data;
	int			i;

	p_data.cmd_num = get_cmd_num(cmds);
	p_data.prev_fd = -1;
	if (p_data.cmd_num <= 0 || cmds == NULL)
		return ;
	if (check_single_builtin(cmds, env, &p_data))
		return ;
	i = 0;
	while (i < p_data.cmd_num)
	{
		if (i < p_data.cmd_num - 1 && pipe(p_data.pipe_fd) == -1)
			exit(EXIT_FAILURE);
		p_data.pid = fork();
		if (p_data.pid == -1)
			exit(EXIT_FAILURE);
		if (p_data.pid == 0)
			child_process(cmds, env, &p_data, i);
		else
			parent_process(&p_data, cmds, env, i);
		i++;
	}
}
