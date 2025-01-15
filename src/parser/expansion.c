#include "minishell.h"

char *ft_strjoin_free(char *s1, char *s2)
{
    char *result;

    if (!s1 && s2)
        return (ft_strdup(s2));
    if (!s2)
        return (s1);
    result = ft_strjoin(s1, s2);
    free(s1);
    return (result);
}

int is_valid_env_char(char c)
{
    return (ft_isalpha(c) || ft_isdigit(c) || c == '_');
}

static void handle_variable_expansion(t_expand_args *args)
{
    if (args->token->value[args->i] == '?')
    {
        args->i++;
        //args->temp = ft_strdup(get_env_value("?", args->env_lst));
        args->temp = ft_itoa(get_status(0, 100));
        args->expand = ft_strjoin_free(args->expand, args->temp);
        free(args->temp);
        return;
    }
    if (args->token->is_quote == 34 && (!args->token->value[args->i] || args->token->value[args->i] == ' ' || !is_valid_env_char(args->token->value[args->i])))
    {
        args->temp = ft_strdup("$");
        args->expand = ft_strjoin_free(args->expand, args->temp);
        free(args->temp);
        return;
    }
    if (args->token->is_quote == 0 && (!args->token->value[args->i] || !is_valid_env_char(args->token->value[args->i])))
    {
        args->temp = ft_strdup("$");
        args->expand = ft_strjoin_free(args->expand, args->temp);
        free(args->temp);
        return;
    }
    args->start = args->i;
    while (args->token->value[args->i] && is_valid_env_char(args->token->value[args->i]))
        args->i++;
    args->temp = ft_strndup(&args->token->value[args->start], args->i - args->start);
    args->sub_expand = get_env_value(args->temp, args->env_lst);
    free(args->temp);
    if (args->token->type == WORD && !args->sub_expand)
        args->sub_expand = NULL;
    args->expand = ft_strjoin_free(args->expand, args->sub_expand);
}

static void handle_literal_text(t_expand_args *args)
{
    args->start = args->i;
    while (args->token->value[args->i] && args->token->value[args->i] != '$')
        args->i++;
    args->temp = ft_strndup(&args->token->value[args->start], args->i - args->start);
    args->expand = ft_strjoin_free(args->expand, args->temp);
    free(args->temp);
}

char *expand_value(t_token *token, t_env **env_lst)
{
    t_expand_args args;

    if (token->prev && token->prev->value && ft_strncmp(token->prev->value, "<<", 2) == 0)
        return (ft_strdup(token->value));

    args.token = token;
    args.env_lst = *env_lst;
    args.expand = NULL;
    args.i = 0;
    args.temp = NULL;
    args.sub_expand = NULL;
    args.start = 0;

    while (token->value[args.i])
    {
        if (token->value[args.i] != '$')
            handle_literal_text(&args);
        else if (token->value[args.i++] == '$')
            handle_variable_expansion(&args);
    }
    return (args.expand);
}


void expand_variables(t_token *token, t_env **env_lst)
{
    char *new_value;

    if (token->expand)
    {
        if (token->type == QUOTED || token->type == WORD)
            new_value = expand_value(token, env_lst);
        else
            return;
        // Libera el valor anterior y actualiza con el nuevo
        free(token->value);
        token->value = new_value;
    }
}