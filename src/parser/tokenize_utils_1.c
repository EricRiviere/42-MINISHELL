/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:15:46 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 19:22:17 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(t_type type)
{
	t_token	*new_tkn;

	new_tkn = malloc(sizeof(t_token));
	if (!new_tkn)
		return_error("token malloc error\n");
	new_tkn->value = NULL;
	new_tkn->is_quote = 0;
	new_tkn->expand = 0;
	new_tkn->hd_fd = 1;
	new_tkn->type = type;
	new_tkn->next = NULL;
	new_tkn->prev = NULL;
	return (new_tkn);
}

void	add_token(t_token **tkn_lst, t_token *token)
{
	t_token	*curr_tkn;

	if (!*tkn_lst)
		*tkn_lst = token;
	else
	{
		curr_tkn = *tkn_lst;
		while (curr_tkn->next)
			curr_tkn = curr_tkn->next;
		curr_tkn->next = token;
		token->prev = curr_tkn;
	}
}

void	free_tkn_lst(t_token *tkn_lst)
{
	t_token	*next;

	while (tkn_lst)
	{
		next = tkn_lst->next;
		if (tkn_lst->value)
			free(tkn_lst->value);
		free(tkn_lst);
		tkn_lst = next;
	}
}

int	create_operator_token(t_args *args, t_operator operator, int increment)
{
	t_token	*token;

	token = parse_operator(operator, args->spaces);
	if (!token)
		return (0);
	*(args->i) += increment;
	add_token(args->tkn_lst, token);
	return (1);
}

int	process_token(t_args *args)
{
	int	start;

	start = *(args->i);
	if (args->line[*(args->i)] == '"' || args->line[*(args->i)] == '\'')
		return (manage_quote(args, start) != NULL);
	if (args->line[*(args->i)] == '<' || args->line[*(args->i)] == '>'
		|| args->line[*(args->i)] == '|')
		return (manage_operator(args) != NULL);
	if (!is_special_char(args->line[*(args->i)]))
		return (manage_word(args) != NULL);
	(*(args->i))++;
	return (1);
}
