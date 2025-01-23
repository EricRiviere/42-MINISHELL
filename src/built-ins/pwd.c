/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:20:49 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:21:57 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printf_pwd(t_env **env)
{
	t_env	*env_lst;
	char	*path;

	env_lst = *env;
	path = NULL;
	while (env_lst)
	{
		if (ft_strncmp(env_lst->key, "PWD", 3) && ft_strlen(env_lst->key) == 3)
			printf("%s\n", env_lst->value);
		env_lst = env_lst->next;
	}
	if (!env_lst)
	{
		path = getcwd(NULL, 0);
		printf("%s\n", path);
		free(path);
	}
}
