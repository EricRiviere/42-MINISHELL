/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:34:19 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 18:44:49 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	add_env_variable(t_env **env_list, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free_env_list(*env_list);
		return ;
	}
	new_node->key = ft_strdup(key);
	if (!new_node)
	{
		free(new_node);
		return ;
	}
	new_node->value = ft_strdup(value);
	if (!new_node)
	{
		free(new_node->key);
		free(new_node);
		return ;
	}
	new_node->next = *env_list;
	*env_list = new_node;
}

t_env	*init_env_list(char **env)
{
	t_env	*env_list;
	char	*separator;
	char	*key;
	char	*value;

	env_list = NULL;
	while (*env)
	{
		separator = ft_strchr(*env, '=');
		if (separator)
		{
			key = ft_strndup(*env, separator - *env);
			value = ft_strdup(separator + 1);
			if (!key || !value)
			{
				double_free_char(key, value);
				free_env_list(env_list);
				return (NULL);
			}
			add_env_variable(&env_list, key, value);
			double_free_char(key, value);
		}
		env++;
	}
	return (env_list);
}

void	print_env_list(t_env *env_list)
{
	while (env_list)
	{
		printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}

char	*get_env_value(char *str, t_env *env_list)
{
	while (env_list)
	{
		if (ft_strncmp(str, env_list->key, ft_strlen(str)) == 0
			&& ft_strlen(str) == ft_strlen(env_list->key))
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}
