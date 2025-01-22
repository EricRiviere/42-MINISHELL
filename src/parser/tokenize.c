/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:08:35 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 19:12:54 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args	*pass_args(char *line, int *i, t_token **tkn_lst, int spaces)
{
	t_args	*args;

	args = malloc(sizeof(t_args));
	if (!args)
		return_error("args malloc error\n");
	args->line = line;
	args->i = i;
	args->tkn_lst = tkn_lst;
	args->spaces = spaces;
	return (args);
}

t_token	*parse_quote(t_args *args, char quote, int start, int end)
{
	char	*str;
	t_token	*tkn;

	str = ft_strndup (&(args->line[start + 1]), end - start - 1);
	if (!str)
		return_error("strndup token error\n");
	tkn = init_token(QUOTED);
	tkn->value = str;
	tkn->is_quote = quote;
	tkn->has_space = args->spaces;
	tkn->expand = (quote == '"' && ft_strchr(str, '$') != NULL);
	return (tkn);
}

t_token	*parse_operator(t_operator operator, int spaces)
{
	char	*name;
	t_token	*tkn;
	char	*operator_name[5];

	operator_name[0] = "<";
	operator_name[1] = ">";
	operator_name[2] = "<<";
	operator_name[3] = ">>";
	operator_name[4] = "|";
	name = ft_strdup(operator_name[operator]);
	if (!name)
		return_error("strdup token error\n");
	tkn = init_token(OPERATOR);
	tkn->value = name;
	tkn->has_space = spaces;
	return (tkn);
}

t_operator	get_operator_type(const char *line, int i)
{
	if (line[i] == '>' && line[i + 1] == '>')
		return (APPEND);
	if (line[i] == '<' && line[i + 1] == '<')
		return (HEREDOC);
	if (line[i] == '>')
		return (OUTPUT);
	if (line[i] == '<')
		return (INPUT);
	return (PIPE);
}

t_token	*tokenize(char *line)
{
	t_token	*tkn_lst;
	int		i;
	int		spaces;
	t_args	*args;

	i = 0;
	tkn_lst = NULL;
	while (line[i])
	{
		skip_spaces(line, &i, &spaces);
		if (line[i] && !is_space(line[i]))
		{
			args = pass_args(line, &i, &tkn_lst, spaces);
			if (!args)
				return (NULL);
			if (!process_token(args))
			{
				free(args);
				return (NULL);
			}
			free(args);
		}
	}
	return (tkn_lst);
}
