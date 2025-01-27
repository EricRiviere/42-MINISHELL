/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:22:44 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 19:28:01 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*manage_quote(t_args *args, int start)
{
	char	quote;
	int		end;
	t_token	*token;

	quote = args->line[(*args->i)++];
	while (args->line[*args->i] && args->line[*args->i] != quote)
		(*args->i)++;
	if (!args->line[*args->i])
	{
		perror("unclosed quote error\n");
		free_tkn_lst(*args->tkn_lst);
		get_status(1, 2);
		return (NULL);
	}
	end = (*args->i)++;
	token = parse_quote(args, quote, start, end);
	add_token(args->tkn_lst, token);
	return (*(args->tkn_lst));
}

t_token	*manage_operator(t_args *args)
{
	int			increment;
	t_operator	operator;

	operator = get_operator_type(args->line, *(args->i));
	if (operator == APPEND || operator == HEREDOC)
		increment = 2;
	else
		increment = 1;
	if (!create_operator_token(args, operator, increment))
		return (NULL);
	return (*(args->tkn_lst));
}

t_token	*manage_word(t_args *args)
{
	int		start;
	int		end;
	int		expand_value;
	char	*word;
	t_token	*tkn;

	expand_value = 0;
	start = (*args->i);
	while (args->line[*args->i] && !is_special_char(args->line[*args->i]))
	{
		if (args->line[*args->i] == '$')
			expand_value = 1;
		(*args->i)++;
	}
	end = (*args->i);
	word = ft_strndup(&(args->line[start]), end - start);
	if (!word)
		return_error("strndup token error\n");
	tkn = init_token(WORD);
	tkn->value = word;
	tkn->has_space = args->spaces;
	tkn->expand = expand_value;
	add_token(args->tkn_lst, tkn);
	return (*(args->tkn_lst));
}

void	skip_spaces(const char *line, int *i, int *spaces)
{
	*spaces = 0;
	while (line[*i] && is_space(line[*i]))
	{
		(*spaces)++;
		(*i)++;
	}
}
