#include "minishell.h"

void execute_pipes(t_command **cmds, t_env **env)
{
    int i;
    int cmd_num;
    int pipe_fd[2];
    int prev_fd = -1; // Para almacenar el extremo de lectura de la tubería anterior
    pid_t pid;

    cmd_num = get_cmd_num(cmds);
    if (cmd_num <= 0 || cmds == NULL)
        return;

    for (i = 0; i < cmd_num; i++)
    {
        if (i < cmd_num - 1)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) // Proceso hijo
        {
            // Redirigir entrada si no es el primer comando
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }

            // Redirigir salida si no es el último comando
            if (i < cmd_num - 1)
            {
                if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipe_fd[1]);
                close(pipe_fd[0]);
            }

            // Manejar redirecciones específicas del comando actual
            if (cmds[i]->fd_in != -1)
            {
                if (dup2(cmds[i]->fd_in, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(cmds[i]->fd_in);
            }
            if (cmds[i]->fd_out != -1)
            {
                if (dup2(cmds[i]->fd_out, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(cmds[i]->fd_out);
            }

            // Ejecutar el comando
            execute_cmd(&cmds[i], env);
            exit(EXIT_FAILURE); // Si `execute_cmd` falla
        }
        else // Proceso padre
        {
            if (prev_fd != -1)
                close(prev_fd); // Cerrar extremo de lectura de la tubería anterior

            if (i < cmd_num - 1)
            {
                close(pipe_fd[1]); // Cerrar extremo de escritura de la tubería actual
                prev_fd = pipe_fd[0]; // Guardar extremo de lectura para el siguiente comando
            }

            // Esperar al hijo opcionalmente
            waitpid(pid, NULL, 0);
        }
    }
    // int term_out = 0;
    // int term_in = 0;
    // if (dup2(term_out, STDOUT_FILENO) == -1)
    // {
    //     perror("dup");
    //     exit(EXIT_FAILURE);
    // }
    // if (dup2(term_in, STDIN_FILENO) == -1)
    // {
    //     perror("dup");
    //     exit(EXIT_FAILURE);
    // }
}