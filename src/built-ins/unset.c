/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:22:52 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:29:42 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_node(t_env **env, t_env *prev, t_env *curr)
{
	if (prev == NULL)
		*env = curr->next;
	else
		prev->next = curr->next;
	free(curr->key);
	free(curr->value);
	free(curr);
}

static void	delete_env_key(t_env **env, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(key, curr->key, -1) == 0)
		{
			remove_env_node(env, prev, curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	delete_env_var(t_command **cmd, t_env **env)
{
	int	i;

	i = 0;
	while ((*cmd)->args[i])
	{
		delete_env_key(env, (*cmd)->args[i]);
		i++;
	}
}
