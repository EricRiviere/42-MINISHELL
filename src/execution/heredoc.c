#include "minishell.h"

// abre fds infinitos pero no se cierra cuando devuelve su nombre
// faltaria un strncmp que chequee si line == heredoc name y se cierre
// hacer un dup para redirigir el output en caso de que se rediriga el heredoc a outfile
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