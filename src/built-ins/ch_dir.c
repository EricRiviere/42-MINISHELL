#include "minishell.h"

void change_dir(t_command *cmd, t_env **env)
{
    char *old_pwd;
    char *new_pwd;
    char *target_dir;
    t_env *env_lst;

    env_lst = *env;
    if (!cmd->args[0] || cmd->args[0][0] == '\0') // Caso sin argumentos
    {
        target_dir = get_env_value("HOME", env_lst);
        if (!target_dir || target_dir[0] == '\0')
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            cmd->status = EXIT_FAILURE;
            return;
        }
    }
    else if (ft_strncmp(cmd->args[0], "-", -1) == 0) // Caso "cd -"
    {
        target_dir = get_env_value("OLDPWD", env_lst);
        if (!target_dir)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            cmd->status = EXIT_FAILURE;
            return;
        }
        ft_putendl_fd(target_dir, 1); // Imprime el valor de OLDPWD
    }
    else
        target_dir = cmd->args[0]; // Caso normal

    old_pwd = getcwd(NULL, 0); // Guarda el directorio actual como OLDPWD
    if (chdir(target_dir) == -1) // Cambia al nuevo directorio
    {
        perror("minishell: cd");
        free(old_pwd);
        cmd->status = EXIT_FAILURE;
        return;
    }
    new_pwd = getcwd(NULL, 0); // Obtén el nuevo directorio como PWD
    if (old_pwd)
    {
        cu_env_var(env, "OLDPWD", old_pwd); // Actualiza OLDPWD
        free(old_pwd);
    }
    if (new_pwd)
    {
        cu_env_var(env, "PWD", new_pwd); // Actualiza PWD
        free(new_pwd);
    }
    cmd->status = EXIT_SUCCESS;
}
