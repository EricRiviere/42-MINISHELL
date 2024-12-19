#include "minishell.h"

int is_valid_key(char c)
{
    return (ft_isalpha(c) || c == '_');
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("%i\n", is_valid_key(argv[1][0]));
    }

}