#include "minishell.h"
int main()
{
    char *str = "ls -l";
    
    int i = 0;
    char **res = ft_split(str, ' ');
    while (res[i])
    {
        printf("%s\n", res[i]);
        i++;
    }


}