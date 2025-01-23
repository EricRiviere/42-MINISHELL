/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eriviere <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:26:04 by eriviere          #+#    #+#             */
/*   Updated: 2025/01/23 14:30:01 by eriviere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*return_error(char *str)
{
	perror(str);
	return (NULL);
}

void	double_free_char(char *s1, char *s2)
{
	if (!s1 || !s2)
		return ;
	free(s1);
	free(s2);
}

char	*ft_strndup(const char *str, size_t len)
{
	char	*dup;
	size_t	s_len;
	size_t	i;

	if (!str)
		return (NULL);
	s_len = ft_strlen(str);
	if (s_len <= len)
		len = s_len;
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

extern inline int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

extern inline int	is_special_char(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '"'
		|| c == '\'' || c == ' ' || (c >= 9 && c <= 13));
}
