
#include "minishell.h"


int process_heredoc(t_token *heredoc_token)
{
    int redir[2];
    int heredoc;
    int status;
    char *line;

    signal(SIGINT, break_it);
	signal(SIGQUIT, SIG_IGN);
    if (get_break_it(0, 0))
        return (130);
    status = 0;
    if (pipe(redir) == -1)
    {
        perror("pipe");
        return -1;
    }

    heredoc = fork();
    if (heredoc == -1)
    {
        perror("fork");
        close(redir[0]);
        close(redir[1]);
        return -1;
    }

    if (heredoc == 0) 
    {
        signal(SIGINT, here_signals);
        close(redir[0]);
        while (1)
        {
            line = readline("> ");
            if (!line || ft_strncmp(heredoc_token->next->value, line, -1) == 0)
            {
                free(line);
                break;
            }
            write(redir[1], line, ft_strlen(line));
            write(redir[1], "\n", 1);
            free(line);
        }
        close(redir[1]);
        signal(SIGINT, SIG_DFL);
        exit(0);
    }
    // signal(SIGINT, SIG_DFL);
    // Parent process
    close(redir[1]);
    if (waitpid(heredoc, &status, 0) == -1)
    {
        perror("waitpid");
        close(redir[0]);
        return -1;
    }

    // if (WIFEXITED((status)))
    //     status = WEXITSTATUS(status);
    // else
    //     handle_signaled(&status, WTERMSIG(status));
    status = WEXITSTATUS(status);
    get_status(1,status);
    if (status != 0)
    {
        close(redir[0]);
        return -1;
    }
    return redir[0]; 
}