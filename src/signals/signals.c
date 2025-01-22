/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:44:57 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 18:44:59 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signaled(int *status, int signal)
{
	if (signal == 2)
		printf("^C\n");
	else if (signal == 3)
		printf("Quit: (core dumped)\n");
	*status = 128 + signal;
}

void	parent_signals(void)
{
	signal(SIGINT, output_signals);
	signal(SIGQUIT, SIG_IGN);
}

void	here_signals(int sig)
{
	if (sig == SIGINT)
	{
		printf(">\n");
		exit(130);
	}
}

void	output_signals(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		get_status(1, 130);
	}
}
