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

void	child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, output_signals);
}

void	here_signals(int sig)
{
	if (sig == SIGINT)
	{
		printf(">\n");
		exit(130);
	}
    // signal(SIGQUIT, SIG_IGN);
}

// void	sigquit_signals(int sig)
// {
// 	(void)sig;
// 	rl_on_new_line();
// 	write(STDOUT_FILENO, "hola", 4);
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

		// ft_printf("\n");
		// rl_replace_line("", 0);
		// rl_on_new_line();
		// rl_redisplay();

void	output_signals(int sig)
{
	
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		// printf("aaaaa\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit: core dump\n", 16);
		// rl_replace_line("", 0);
	}
	//rl_redisplay();  // Redibujar el prompt
}