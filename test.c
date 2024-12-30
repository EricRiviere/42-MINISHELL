#include "minishell.h"

// int is_valid_key(char c)
// {
//     return (ft_isalpha(c) || c == '_');
// }

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}


char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	i = 0;
	dest = malloc(sizeof(char) * (len + 1));
	if (dest == NULL)
		return (NULL);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        // printf("%i\n", is_valid_key(argv[1][0]));
        printf("%s\n", ft_strdup(NULL));
    }

}