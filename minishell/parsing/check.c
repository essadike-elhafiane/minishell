/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 23:08:37 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/07 23:10:19 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_is_oper(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == 34 || c == 39)
		return (1);
	return (0);
}

int	check_is_oper_error(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

int	checke_pipe(char *str)
{
	int	i;
	int	flg;

	i = 0;
	flg = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			i++;
			while (str[i] == ' ')
				i++;
			if (str[i] == '\0')
				return (1);
		}
		else
			i++;
	}
	return (flg);
}

int	checke_double(char *str)
{
	int	i;
	int	flg;

	i = 0;
	flg = 0;
	while (str[i])
	{
		if (str[i] == 34)
			flg++;
		i++;
	}
	return (flg);
}

int	checke_single(char *str)
{
	int	i;
	int	flg;

	i = 0;
	flg = 0;
	while (str[i])
	{
		if (str[i] == 39)
			flg++;
		i++;
	}
	return (flg);
}
