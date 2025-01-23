/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:14:38 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 11:24:53 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && s2)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

int	is_valid_env_char(char c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || c == '_');
}

char	*expand_value(t_token *token, t_env **env_lst)
{
	t_expand_args	args;

	if (token->prev && token->prev->value
		&& ft_strncmp(token->prev->value, "<<", 2) == 0)
		return (ft_strdup(token->value));
	args.token = token;
	args.env_lst = *env_lst;
	args.expand = NULL;
	args.i = 0;
	args.temp = NULL;
	args.sub_expand = NULL;
	args.start = 0;
	while (token->value[args.i])
	{
		if (token->value[args.i] != '$')
			handle_literal_text(&args);
		else if (token->value[args.i++] == '$')
			handle_variable_expansion(&args);
	}
	return (args.expand);
}

void	expand_variables(t_token *token, t_env **env_lst)
{
	char	*new_value;

	if (token->expand)
	{
		if (token->type == QUOTED || token->type == WORD)
			new_value = expand_value(token, env_lst);
		else
			return ;
		free(token->value);
		token->value = new_value;
	}
}
