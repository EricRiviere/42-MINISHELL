#include "minishell.h"

void process_heredoc(t_token *heredoc_token)
{
    int fd;
    char *line;
    fd = open("/tmp/heredoc_pipe", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("Error opening heredoc", 2);
        return ;
    }
    while (1)
    {
        line = readline(">");
        if ((ft_strncmp(line, heredoc_token->next->value, ft_strlen(heredoc_token->next->value)) == 0)
            && ft_strlen(line) == ft_strlen(heredoc_token->next->value))
        {
            free(line);
            break;
        }
        if (line == NULL)
            break ;
        write(fd, line, ft_strlen(line));
        write(fd, "\n" ,1);
        free(line);
    }
    close(fd);
}