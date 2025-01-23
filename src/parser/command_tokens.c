/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:30:20 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 12:52:48 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concat_tokens(t_token **curr, t_token **prev)
{
	char	*temp;

	temp = (*prev)->value;
	(*prev)->value = ft_strjoin((*prev)->value, (*curr)->value);
	free(temp);
	(*prev)->next = (*curr)->next;
	if ((*curr)->next)
		(*curr)->next->prev = *prev;
	free((*curr)->value);
	free(*curr);
	*curr = (*prev)->next;
}

void	preprocess_tokens(t_token **tkn_lst)
{
	t_token	*curr;
	t_token	*prev;

	curr = *tkn_lst;
	while (curr)
	{
		if ((curr->type == WORD || curr->type == QUOTED)
			&& curr->has_space == 0)
		{
			prev = curr->prev;
			if (prev && (prev->type == WORD || prev->type == QUOTED))
			{
				concat_tokens(&curr, &prev);
				continue ;
			}
		}
		curr = curr->next;
	}
}

void	manage_redirection_cmd(t_token **curr_tkn, t_redir *redir,
		int *op_index)
{
	if ((*curr_tkn)->value && (*curr_tkn)->next->value)
		add_redirection(redir, *curr_tkn, op_index);
	*curr_tkn = (*curr_tkn)->next->next;
}

void	manage_word_quote_cmd(t_command *cmd, t_token **curr_tkn,
		int *arg_index)
{
	if ((*curr_tkn)->value)
	{
		if (!cmd->cmd)
			cmd->cmd = ft_strdup((*curr_tkn)->value);
		else
			cmd->args[(*arg_index)++] = ft_strdup((*curr_tkn)->value);
	}
	*curr_tkn = (*curr_tkn)->next;
}

void	process_tokens(t_command *cmd, t_redir *redir, t_token **curr_tkn)
{
	int	arg_index;
	int	op_index;

	arg_index = 0;
	op_index = 0;
	while (*curr_tkn && !((*curr_tkn)->type == 2
			&& ft_strncmp((*curr_tkn)->value, "|", 1) == 0))
	{
		if ((*curr_tkn)->type == 2 && (*curr_tkn)->next)
			manage_redirection_cmd(curr_tkn, redir, &op_index);
		else if ((*curr_tkn)->type == 0 || (*curr_tkn)->type == 1)
			manage_word_quote_cmd(cmd, curr_tkn, &arg_index);
		else
			*curr_tkn = (*curr_tkn)->next;
	}
	cmd->args[arg_index] = NULL;
	redir->operator[op_index] = NULL;
	redir->file[op_index] = NULL;
	if (process_redirections(cmd, redir) == -1)
	{
		cmd->redir_error = 1;
		return ;
	}
	cmd->redir_error = 0;
}
