/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:45:18 by gualvare          #+#    #+#             */
/*   Updated: 2025/01/22 19:03:58 by gualvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_status(int flag, int value)
{
	static int	new;

	if (flag == 1)
		new = value;
	return (new);
}

int	get_break_it(int flag, int value)
{
	static int	_break;

	if (flag == 1)
		_break = value;
	return (_break);
}

void	break_it(int signal)
{
	if (signal == SIGINT)
		get_break_it(1, 1);
}
