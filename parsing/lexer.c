/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 23:00:51 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/13 23:25:03 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	lexer_0(char *str, t_stk *y)
{
	if (str[y->i] == 32)
	{
		y->ss = ft_strdup(" ");
		y->tmp->next = init_data(y->ss, WSPACE);
		y->tmp = y->tmp->next;
		free(y->ss);
		while (str[y->i] == ' ')
			y->i++;
		y->i--;
	}
}

void	lexer_1(char *str, t_stk *y)
{
	if (str[y->i] == '|')
	{
		y->ss = ft_strdup("|");
		y->tmp->next = init_data(y->ss, PIPE);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (str[y->i] == 34)
	{
		y->i++;
		y->b = y->i;
		while (str[y->i] != 34)
			y->i++;
		y->ss = ft_substr(str, y->b, y->i - y->b);
		y->tmp->next = init_data(y->ss, DOUBLE);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (str[y->i] == '>' && str[y->i + 1] != '>')
	{
		y->ss = ft_strdup(">");
		y->tmp->next = init_data(y->ss, OUT);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
}

void	lexer_2(char *str, t_stk *y)
{
	if (str[y->i] == '>' && str[y->i + 1] == '>')
	{
		y->ss = ft_strdup(">>");
		y->tmp->next = init_data(y->ss, APPEND);
		y->tmp = y->tmp->next;
		free(y->ss);
		y->i++;
	}
	if (str[y->i] == '<' && str[y->i + 1] != '<')
	{
		y->ss = ft_strdup("<");
		y->tmp->next = init_data(y->ss, IN);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (str[y->i] == '<' && str[y->i + 1] == '<')
	{
		y->ss = ft_strdup("<<");
		y->tmp->next = init_data(y->ss, HER);
		y->tmp = y->tmp->next;
		free(y->ss);
		y->i++;
	}
}

void	lexer_3(char *str, t_stk *y)
{
	if (str[y->i] == 39)
	{
		y->i++;
		y->b = y->i;
		while (str[y->i] != 39)
			y->i++;
		y->ss = ft_substr(str, y->b, y->i - y->b);
		y->tmp->next = init_data(y->ss, SINGLE);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (!check_is_oper(str[y->i]) && str[y->i] != ' ')
	{
		y->j = y->i;
		while (!check_is_oper(str[y->i]) && str[y->i] && str[y->i] != ' ')
			y->i++;
		y->ss = ft_substr(str, y->j, y->i - y->j);
		y->tmp->next = init_data(y->ss, WORD);
		y->tmp = y->tmp->next;
		free(y->ss);
		y->i--;
	}
}

void	lexer(char *str, t_env **envs)
{
	t_stk	y;

	y.i = 0;
	if (!str || str[0] == '\0')
		return ;
	y.ss = ft_strdup("1");
	y.data_cmd = init_data(y.ss, 14);
	y.tmp = y.data_cmd;
	free(y.ss);
	while (str[y.i] == ' ' || str[y.i] == '\t')
		y.i++;
	while (str[y.i])
	{
		lexer_0(str, &y);
		lexer_1(str, &y);
		lexer_2(str, &y);
		lexer_3(str, &y);
		y.i++;
	}
	lexer_help(&y, envs);
}
