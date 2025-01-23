/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_dir.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:51:14 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:02:14 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exit_status(t_env **env, int status)
{
	char	*status_str;

	status_str = ft_itoa(get_status(1, status));
	cu_env_var(env, "?", status_str);
	free(status_str);
}

char	*print_err_ret_null(char *err_message, t_env **env)
{
	ft_putstr_fd(err_message, 2);
	update_exit_status(env, EXIT_FAILURE);
	return (NULL);
}

char	*get_cd_target(t_command *cmd, t_env *env_lst)
{
	char	*target_dir;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
	{
		target_dir = get_env_value("HOME", env_lst);
		if (!target_dir || target_dir[0] == '\0')
			return (print_err_ret_null("minishell: cd: HOME not set\n",
					&env_lst));
	}
	else if (ft_strncmp(cmd->args[0], "-", -1) == 0)
	{
		target_dir = get_env_value("OLDPWD", env_lst);
		if (!target_dir)
			return (print_err_ret_null("minishell: cd: OLDPWD not set\n",
					&env_lst));
		ft_putendl_fd(target_dir, 1);
	}
	else
		target_dir = cmd->args[0];
	return (target_dir);
}

void	update_pwd(t_env **env, char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
	{
		cu_env_var(env, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		cu_env_var(env, "PWD", new_pwd);
		free(new_pwd);
	}
}

void	change_dir(t_command *cmd, t_env **env)
{
	char	*old_pwd;
	char	*target_dir;

	target_dir = get_cd_target(cmd, *env);
	if (!target_dir)
		return ;
	old_pwd = getcwd(NULL, 0);
	if (chdir(target_dir) == -1)
	{
		perror("minishell: cd");
		free(old_pwd);
		update_exit_status(env, EXIT_FAILURE);
		return ;
	}
	update_pwd(env, old_pwd);
}
