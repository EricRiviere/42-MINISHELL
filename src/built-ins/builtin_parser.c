#include "minishell.h"

void builtin_manager(t_command **cmd, t_env **env)
{
    if ((*cmd)->cmd == "export")
        export_new_env(cmd, &env);
    else if ((*cmd)->cmd == "CD")
        //funcion gestion de CD;
    else if ((*cmd)->cmd == "echo")
        //gestion de echo;
    else if ((*cmd)->cmd == "unset")
        // gestion de unset;
    else if ((*cmd)->cmd == "pwd")
        // gestion de pwd;
    else if ((*cmd)->cmd == "env")
        // gestion de env;
    else if ((*cmd)->cmd == "exit")
        // gestion exit;

}