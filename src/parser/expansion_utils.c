/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:28:14 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 11:28:17 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_exit_status(t_expand_args *args)
{
	args->i++;
	args->temp = ft_itoa(get_status(0, 100));
	args->expand = ft_strjoin_free(args->expand, args->temp);
	free(args->temp);
}

static void	expand_single_dollar(t_expand_args *args)
{
	args->temp = ft_strdup("$");
	args->expand = ft_strjoin_free(args->expand, args->temp);
	free(args->temp);
}

static void	expand_env_variable(t_expand_args *args)
{
	args->start = args->i;
	while (args->token->value[args->i]
		&& is_valid_env_char(args->token->value[args->i]))
		args->i++;
	args->temp = ft_strndup(&args->token->value[args->start],
			args->i - args->start);
	args->sub_expand = get_env_value(args->temp, args->env_lst);
	free(args->temp);
	if (args->token->type == WORD && !args->sub_expand)
		args->sub_expand = NULL;
	args->expand = ft_strjoin_free(args->expand, args->sub_expand);
}

void	handle_variable_expansion(t_expand_args *args)
{
	if (args->token->value[args->i] == '?')
	{
		expand_exit_status(args);
		return ;
	}
	if ((args->token->is_quote == 34 || args->token->is_quote == 0)
		&& (!args->token->value[args->i]
			|| !is_valid_env_char(args->token->value[args->i])
			|| (args->token->is_quote == 34
				&& args->token->value[args->i] == ' ')))
	{
		expand_single_dollar(args);
		return ;
	}
	expand_env_variable(args);
}

void	handle_literal_text(t_expand_args *args)
{
	args->start = args->i;
	while (args->token->value[args->i] && args->token->value[args->i] != '$')
		args->i++;
	args->temp = ft_strndup(&args->token->value[args->start],
			args->i - args->start);
	args->expand = ft_strjoin_free(args->expand, args->temp);
	free(args->temp);
}
