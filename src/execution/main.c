/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:17:42 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 18:18:08 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_line_too_long(char *line)
{
	ft_putendl_fd("Error: Line too long to execute",
		STDERR_FILENO);
	free(line);
}

void	process_tok(t_token **tkn_lst, t_env **env_lst)
{
	t_token	*curr_tkn;

	curr_tkn = *tkn_lst;
	while (curr_tkn)
	{
		if (ft_strncmp(curr_tkn->value, "<<", 2) == 0)
			curr_tkn->hd_fd = process_heredoc(curr_tkn);
		expand_variables(curr_tkn, env_lst);
		curr_tkn = curr_tkn->next;
	}
	get_break_it(1, 0);
	preprocess_tokens(tkn_lst);
	if (!get_status(0, 0))
		cu_env_var(env_lst, "?", 0);
}

void	process_line(char *line, t_env **env_lst)
{
	t_token		*tkn_lst;
	t_command	**cmd_list;

	add_history(line);
	tkn_lst = tokenize(line);
	if (!tkn_lst || syntax_check(tkn_lst))
	{
		process_tok(&tkn_lst, env_lst);
		cmd_list = commands(tkn_lst);
		//print_commands(line, cmd_list);
		//print_tokens(line, tkn_lst);
		execute_pipes(cmd_list, env_lst);
		free_cmd_list(cmd_list);
	}
	free_tkn_lst(tkn_lst);
}

int	main(int argc, char **argv, char **env)
{
	t_env		*env_lst;
	char		*line;

	(void)argv;
	if (argc != 1)
		return (printf("Wrong number of arguments\n"), 1);
	env_lst = init_env_list(env);
	parent_signals();
	while (1)
	{
		parent_signals();
		line = readline("minishell> ");
		if (!line)
			break ;
		if ((int)ft_strlen(line) >= 1000)
			handle_line_too_long(line);
		if (*line)
			process_line(line, &env_lst);
		free(line);
	}
	free_env_list(env_lst);
	return (0);
}
