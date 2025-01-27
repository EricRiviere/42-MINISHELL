/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:28:34 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 17:28:36 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_num(t_command **cmd)
{
	int	num;
	int	idx;

	num = 0;
	idx = 0;
	if (cmd == NULL || *cmd == NULL)
		return (num);
	if ((*cmd)->cmd == NULL)
		return (num);
	while (cmd[idx] != NULL)
	{
		num++;
		idx++;
	}
	return (num);
}

void	cmd_not_found(t_command **cmd, char **new_arr)
{
	ft_putstr_fd("command not found : ", 2);
	ft_putstr_fd((*cmd)->cmd, 2);
	ft_putstr_fd("\n", 2);
	free_array(new_arr);
}

void	execute_cmd(t_command **cmd, t_env **env)
{
	char	**new_arr;
	int		cmd_num;

	if (!cmd || !(*cmd)->cmd)
		return ;
	cmd_num = get_cmd_num(cmd);
	if (cmd_num == 1 && is_builtin(*cmd) != 0)
	{
		manage_builtins(*cmd, env);
		if ((*cmd)->fd_out != -1)
			close((*cmd)->fd_out);
	}
	else
	{
		new_arr = new_args(cmd);
		if ((*cmd)->cmd[0] == '/')
			handle_absolute_path(cmd, new_arr);
		else if ((*cmd)->cmd[0] == '.')
			handle_relative_path(cmd, env, new_arr);
		execute_path(cmd, env, new_arr);
	}
}
