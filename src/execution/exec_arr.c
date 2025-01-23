/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_arr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:28:25 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 17:28:26 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arr_len(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
		i++;
	return (i);
}

char	**new_args(t_command **cmd)
{
	int		len;
	int		i;
	int		j;
	char	**new_arr;

	i = 1;
	j = 0;
	if ((*cmd)->cmd == NULL)
		return (NULL);
	len = arr_len((*cmd)->args);
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (new_arr == NULL)
		return (NULL);
	new_arr[0] = ft_strdup((*cmd)->cmd);
	while ((*cmd)->args[j])
		new_arr[i++] = ft_strdup((*cmd)->args[j++]);
	new_arr[i] = NULL;
	return (new_arr);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**create_env_array(t_env *env_list, int list_len)
{
	char	**arr;
	int		i;
	char	*temp;
	char	*var;

	arr = malloc(sizeof(char *) * (list_len + 1));
	if (!arr)
		return (perror("malloc error"), NULL);
	i = 0;
	while (env_list)
	{
		temp = ft_strjoin(env_list->key, "=");
		if (!temp)
			return (perror("malloc error"), free(arr), NULL);
		var = ft_strjoin(temp, env_list->value);
		free(temp);
		if (!var)
			return (perror("malloc error"), free(arr), NULL);
		arr[i++] = var;
		env_list = env_list->next;
	}
	arr[i] = NULL;
	return (arr);
}

char	**env_arr(t_env *env_list)
{
	int		list_len;
	t_env	*temp;

	list_len = 0;
	temp = env_list;
	while (temp)
	{
		temp = temp->next;
		list_len++;
	}
	return (create_env_array(env_list, list_len));
}
