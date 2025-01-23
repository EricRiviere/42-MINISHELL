/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:28:45 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 17:28:46 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(t_env *env)
{
	t_env	*curr;
	char	*path;

	curr = env;
	path = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, "PATH", 4) == 0)
		{
			path = curr->value;
			break ;
		}
		curr = curr->next;
	}
	if (!path)
	{
		ft_putstr_fd("minishell: PATH not set\n", 2);
		return (NULL);
	}
	return (path);
}

void	handle_absolute_path(t_command **cmd, char **new_arr)
{
	if (access((*cmd)->cmd, F_OK | X_OK | R_OK) == 0)
		execve((*cmd)->cmd, new_arr, NULL);
	cmd_not_found(cmd, new_arr);
	exit(127);
}

void	handle_relative_path(t_command **cmd, t_env **env, char **new_arr)
{
	char	**arr;

	arr = env_arr(*env);
	if (access((*cmd)->cmd, F_OK | X_OK | R_OK) == 0)
		execve((*cmd)->cmd, new_arr, arr);
	cmd_not_found(cmd, new_arr);
	free_array(arr);
	exit(127);
}

char	*build_full_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

void	execute_path(t_command **cmd, t_env **env, char **new_arr)
{
	char	*path;
	char	**paths;
	int		i;
	char	**arr;
	char	*full_path;

	path = find_path(*env);
	paths = ft_split(path, ':');
	if (!paths)
		return (free_array(new_arr), exit(127));
	arr = env_arr(*env);
	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], (*cmd)->cmd);
		if (access(full_path, F_OK | X_OK | R_OK) == 0)
			execve(full_path, new_arr, arr);
		free(full_path);
		i++;
	}
	cmd_not_found(cmd, new_arr);
	exit(127);
}
