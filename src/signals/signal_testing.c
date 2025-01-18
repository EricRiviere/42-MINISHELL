#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>


void	output_signals(int sig)
{
	(void)sig;
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 0);
	rl_redisplay();
}

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



void	display_new_line(int sig)
{
	(void)sig;
	printf("cortooooooooooooo\n");
	rl_on_new_line();
}

int main()
{
    int id = fork();
    int i = 0;
    int status = 0;

    parent_signals();
    if (id == 0)
    {
        child_signals();
        while (i < 5)
        {
            sleep(1000);
        }
        i++;
    }
    else
        waitpid(id, &status, 0);

}