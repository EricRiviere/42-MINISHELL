#include "minishell.h"

void change_dir(const char *path, t_env **env)
{
    char *old_pwd;
    char *new_pwd;
    t_env *env_lst;

    env_lst = *env;
    if (!path || path[0] == '\0')// Si el path es NULL o vacío, usamos el valor de HOME
        path = get_env_value("HOME", env_lst);
    if (!path || path[0] == '\0') // Si HOME no está definido
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", 2);
        return;
    }
    if (ft_strncmp(path, "-", 1) == 0 && ft_strlen(path) == ft_strlen("-"))// Caso "cd -": cambiar a OLDPWD
    {
        path = get_env_value("OLDPWD", env_lst);
        if (!path)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            return;
        }
        printf("%s\n", path); // Imprimir OLDPWD en stdout
    }
    old_pwd = get_env_value("PWD", env_lst);// Guardamos el valor de PWD actual como OLDPWD
    if (chdir(path) == -1)// Cambiamos de directorio con chdir
    {
        perror("minishell: cd"); // Mostrar el error específico
        return;
    }
    new_pwd = getcwd(NULL, 0);// Obtenemos el nuevo directorio con getcwd  (reserva memoria, hay que liberar)
    if (!new_pwd)
    {
        perror("minishell: cd");
        return;
    }
    if (old_pwd)// Actualizamos las variables de entorno
        cu_env_var(env, "OLDPWD", old_pwd);
    cu_env_var(env, "PWD", new_pwd);
    free(new_pwd);
}