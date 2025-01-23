/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:00:00 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 11:03:35 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_pipe_error(void)
{
	perror("pipe");
	return (-1);
}

static int	handle_fork_error(int *redir)
{
	perror("fork");
	close(redir[0]);
	close(redir[1]);
	return (-1);
}

static void	read_heredoc(t_token *heredoc_token, int *redir)
{
	char	*line;

	signal(SIGINT, here_signals);
	close(redir[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(heredoc_token->next->value, line, -1) == 0)
		{
			free(line);
			break ;
		}
		write(redir[1], line, ft_strlen(line));
		write(redir[1], "\n", 1);
		free(line);
	}
	close(redir[1]);
	signal(SIGINT, SIG_DFL);
	exit(0);
}

static int	handle_parent_process(int *redir, int heredoc)
{
	int	status;

	close(redir[1]);
	if (waitpid(heredoc, &status, 0) == -1)
	{
		perror("waitpid");
		close(redir[0]);
		return (-1);
	}
	status = WEXITSTATUS(status);
	get_status(1, status);
	if (status != 0)
	{
		close(redir[0]);
		return (-1);
	}
	return (redir[0]);
}

int	process_heredoc(t_token *heredoc_token)
{
	int	redir[2];
	int	heredoc;

	signal(SIGINT, break_it);
	signal(SIGQUIT, SIG_IGN);
	if (get_break_it(0, 0))
		return (130);
	if (pipe(redir) == -1)
		return (handle_pipe_error());
	heredoc = fork();
	if (heredoc == -1)
		return (handle_fork_error(redir));
	if (heredoc == 0)
		read_heredoc(heredoc_token, redir);
	return (handle_parent_process(redir, heredoc));
}
