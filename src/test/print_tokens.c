/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:43:35 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 15:44:00 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_token_details(t_token *token)
{
	printf("Token value: *%s*\n", token->value);
	printf("  - is quote? %d\n", token->is_quote);
	printf("  - expands? %d\n", token->expand);
	printf("  - Number of spaces: %d\n", token->has_space);
	printf("  - Type: %u\n", token->type);
}

void	print_tokens(char *line, t_token *tkn_lst)
{
	t_token	*curr_tkn;

	printf("\n=== Input Line ===\n%s\n", line);
	printf("=== Tokens ===\n");
	curr_tkn = tkn_lst;
	while (curr_tkn)
	{
		print_token_details(curr_tkn);
		curr_tkn = curr_tkn->next;
	}
	printf("===================\n");
}
