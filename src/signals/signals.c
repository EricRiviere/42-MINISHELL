#include "minishell.h"

void	parent_signals(void)
{
	printf("que esta pasando");
	signal(SIGINT, output_signals);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signals(void)
{
	printf("llego a child signal");
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// void	child_signals(void)
// {
// 	printf("entrando a child signals");
// 	signal(SIGINT, display_new_line);
// 	signal(SIGQUIT, display_new_line);
// }

void	here_signals(void)
{
	printf("entrando en senales de heredoc");
	signal(SIGINT, SIG_DFL); 
    signal(SIGQUIT, SIG_IGN);
}

void	output_signals(int sig)
{
	(void)sig;
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 0);
	rl_redisplay();
}

void	display_new_line(int sig)
{
	(void)sig;
	printf("cortooooooooooooo\n");
	rl_on_new_line();
}