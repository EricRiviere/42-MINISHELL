/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:47:47 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 14:12:20 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key(char *arg)
{
	int	j;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	return (ft_strndup(arg, j));
}

char	*extract_value(char *arg)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign && equal_sign[1])
		return (ft_strdup(equal_sign + 1));
	return (ft_strdup(""));
}

void	process_argument(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*curr_node;

	key = extract_key(arg);
	if (!key)
		return ;
	if (ft_strchr(arg, '='))
	{
		value = extract_value(arg);
		cu_env_var(env, key, value);
		free(value);
	}
	else
	{
		curr_node = get_var(env, key);
		if (!curr_node)
			cu_env_var(env, key, NULL);
	}
	free(key);
}

void	export_new_var(t_command **cmd, t_env **env)
{
	int		i;
	char	*val;

	i = 0;
	while ((*cmd)->args[i])
	{
		if (is_valid_key((*cmd)->args[i][0]))
			process_argument((*cmd)->args[i], env);
		else
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			val = ft_itoa(get_status(1, EXIT_FAILURE));
			cu_env_var(env, "?", val);
			free(val);
		}
		i++;
	}
}
