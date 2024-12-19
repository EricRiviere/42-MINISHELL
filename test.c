#include "minishell.h"


char *find(t_command *cmd)
{

}
int main()
{
    char *str = "ls";
    char *args[3] = {" ", "-l", "a"};
    
    int i = 0;
    char *new_str = ft_strjoin(str, args[0]);
    printf("%s\n", new_str);
    char *new_str2 = ft_strjoin(new_str, args[1]);
     printf("%s\n", new_str2);
    char **res = ft_split(str, ' ');
    while (res[i])
    {
        printf("%s\n", res[i]);
        i++;
    }


}