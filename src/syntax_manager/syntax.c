/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:29:58 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 10:33:12 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_token *tkn_lst)
{
	t_token	*current;

	current = tkn_lst;
	while (current)
	{
		if (current->type == 2)
		{
			if (current->value[0] == '|')
			{
				if (!current->next || current == tkn_lst)
					return (get_status(1, 2),
						ft_putstr_fd("Syntax err: invalid pipe\n", 2), 0);
			}
			else if (current->value[0] == '>' || current->value[0] == '<')
			{
				if (!current->next || !(current->next->type == WORD
						|| current->next->type == QUOTED))
					return (get_status(1, 2),
						ft_putstr_fd("Syntax error: invalid red\n", 2), 0);
			}
		}
		current = current->next;
	}
	return (1);
}
