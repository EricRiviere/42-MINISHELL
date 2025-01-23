/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:47:47 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 14:21:09 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_key(char c)
{
	return (ft_isalpha(c) || c == '_');
}

t_env	*get_var(t_env **env, char *key)
{
	t_env	*curr_node;

	curr_node = *env;
	while (curr_node)
	{
		if (ft_strncmp(key, curr_node->key, -1) == 0)
			return (curr_node);
		else
			curr_node = curr_node->next;
	}
	return (NULL);
}

void	add_env_variable_with_null(t_env **env_list, char *key)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free_env_list(*env_list);
		perror("malloc error creating env node\n");
		return ;
	}
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		perror("malloc error for env key\n");
		return ;
	}
	new_node->value = NULL;
	new_node->next = *env_list;
	*env_list = new_node;
}

void	cu_env_var(t_env **env, char *key, char *value)
{
	t_env	*curr_node;

	curr_node = get_var(env, key);
	if (curr_node)
	{
		if (curr_node->value)
			free(curr_node->value);
		if (value)
			curr_node->value = ft_strdup(value);
		else
			curr_node->value = NULL;
	}
	else
	{
		if (value)
			add_env_variable(env, key, value);
		else
			add_env_variable_with_null(env, key);
	}
}

void	print_declared_env(t_env **env_list)
{
	t_env	*curr_var;

	if (!env_list || !(*env_list))
		return ;
	curr_var = *env_list;
	while (curr_var)
	{
		if (!curr_var->key)
		{
			curr_var = curr_var->next;
			continue ;
		}
		if (ft_strncmp(curr_var->key, "?", -1) == 0)
		{
			curr_var = curr_var->next;
			continue ;
		}
		if (!curr_var->value)
			printf("declare -x %s\n", curr_var->key);
		else if (curr_var->value[0] == '\0')
			printf("declare -x %s=\"\"\n", curr_var->key);
		else
			printf("declare -x %s=\"%s\"\n", curr_var->key, curr_var->value);
		curr_var = curr_var->next;
	}
}
