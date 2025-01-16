#include "minishell.h"

int is_unique_builtin(t_command *cmd)
{
    if (ft_strncmp(&cmd->cmd[0], "cd", -1) == 0)
        return (2);
    else if (ft_strncmp(&cmd->cmd[0], "export", -1) == 0)
        return (3);
    else if (ft_strncmp(&cmd->cmd[0], "unset", -1) == 0)
        return (4);
    else if (ft_strncmp(&cmd->cmd[0], "exit", -1) == 0)
        return (6);
    else
        return (0);
}


// typedef struct s_exec_params
// {
//     t_command *cmd;
//     t_env **env;
//     int *pipe_fd;
//     int prev_fd;
//     int is_last_cmd;
// } t_exec_params;

// static pid_t protect_fork(void)
// {
//     pid_t pid;

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }
//     return pid;
// }

// static void redirect_input(t_command *cmd, int prev_fd)
// {
//     if (prev_fd != -1)
//     {
//         if (dup2(prev_fd, STDIN_FILENO) == -1)
//         {
//             perror("dup2");
//             exit(EXIT_FAILURE);
//         }
//         close(prev_fd);
//     }
//     if (cmd->fd_in != -1)
//     {
//         if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
//         {
//             perror("dup2");
//             exit(EXIT_FAILURE);
//         }
//         close(cmd->fd_in);
//     }
// }

// static void redirect_output(t_command *cmd, int *pipe_fd, int is_last_cmd)
// {
//     if (!is_last_cmd)
//     {
//         if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
//         {
//             perror("dup2");
//             exit(EXIT_FAILURE);
//         }
//         close(pipe_fd[1]);
//         close(pipe_fd[0]);
//     }
//     if (cmd->fd_out != -1)
//     {
//         if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
//         {
//             perror("dup2");
//             exit(EXIT_FAILURE);
//         }
//         close(cmd->fd_out);
//     }
// }

// static void execute_child(t_exec_params *params)
// {
//     redirect_input(params->cmd, params->prev_fd);
//     redirect_output(params->cmd, params->pipe_fd, params->is_last_cmd);
//     execute_cmd(&params->cmd, params->env);
//     exit(EXIT_FAILURE);
// }

// static void create_pipe(int *pipe_fd, int is_last_cmd)
// {
//     if (!is_last_cmd)
//     {
//         if (pipe(pipe_fd) == -1)
//         {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }
//     }
// }

// static void parent_cleanup(int *prev_fd, int *pipe_fd, int is_last_cmd, pid_t pid)
// {
//     if (*prev_fd != -1)
//         close(*prev_fd);
//     if (!is_last_cmd)
//     {
//         close(pipe_fd[1]);
//         *prev_fd = pipe_fd[0];
//     }
//     waitpid(pid, NULL, 0);
// }

// static void process_command(t_command **cmds, t_env **env, int i, int *prev_fd)
// {
//     int pipe_fd[2];
//     pid_t pid;
//     t_exec_params params;

//     create_pipe(pipe_fd, cmds[i + 1] == NULL);
//     pid = protect_fork();
//     if (pid == 0)
//     {
//         params = (t_exec_params){cmds[i], env, pipe_fd, *prev_fd, cmds[i + 1] == NULL};
//         execute_child(&params);
//     }
//     else
//         parent_cleanup(prev_fd, pipe_fd, cmds[i + 1] == NULL, pid);
// }

// void execute_pipes(t_command **cmds, t_env **env)
// {
//     int prev_fd;
//     int cmd_num;
//     int i;

//     prev_fd = -1;
//     cmd_num = get_cmd_num(cmds);
//     if (cmd_num <= 0 || cmds == NULL)
//         return;
//     if (cmd_num == 1 && is_builtin(cmds[0]))
//     {
//         execute_cmd(&cmds[0], env);
//         return;
//     }
//     i = 0;
//     while (cmds[i])
//     {
//         process_command(cmds, env, i, &prev_fd);
//         i++;
//     }
// }



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

    if (cmd_num == 1 && is_unique_builtin(cmds[0]))
    {
        execute_cmd(&cmds[0], env);
        return ;
    }

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
            exit((*cmds)->status); // If `execute_cmd` fails
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
            waitpid(pid, &(*cmds)->status, 0);
            (*cmds)->status = WEXITSTATUS((*cmds)->status);
            char *new_var = (ft_itoa(get_status(1, (*cmds)->status)));
            cu_env_var(env, "?", new_var);
            free(new_var);

        }
    }
}