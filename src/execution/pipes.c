#include "minishell.h"

void execute_pipes(t_command **cmds, t_env **env)
{
    int i;
    int cmd_num;
    int pipe_fd[2];
    int prev_fd = -1; // To store the read end of the previous pipe
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

        if (pid == 0) // Child process
        {
            // Redirect input if not the first command
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }

            // Redirect output if not the last command
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

            // Handle specific redirections for the current command
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

            // Execute the command
            execute_cmd(&cmds[i], env);
            exit(EXIT_FAILURE); // If `execute_cmd` fails
        }
        else // Parent process
        {
            if (prev_fd != -1)
                close(prev_fd); // Close the read end of the previous pipe

            if (i < cmd_num - 1)
            {
                close(pipe_fd[1]); // Close the write end of the current pipe
                prev_fd = pipe_fd[0]; // Save the read end for the next command
            }

            // Optionally wait for the child process
            waitpid(pid, NULL, 0);
        }
    }
}