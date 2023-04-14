/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:07:00 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/14 18:31:42 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	lexer_0(char *str, t_stk *y)
{
	if (str[y->i] == 32)
	{
		y->ss = ft_strdup(" ");
		y->tmp->next = init_data(y->ss, SPACE);
		y->tmp = y->tmp->next;
		free(y->ss);
		while (str[y->i] == ' ')
			y->i++;
		y->i--;
	}
	if (str[y->i] == '|')
	{
		y->ss = ft_strdup("|");
		y->tmp->next = init_data(y->ss, PIPE);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (str[y->i] == 34)
	{
		y->ss = ft_strdup("\"");
		y->tmp->next = init_data(y->ss, DOUBLE);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
}

void	lexer_1(char *str, t_stk *y)
{
	if (str[y->i] == '>' && str[y->i + 1] != '>')
	{
		y->ss = ft_strdup(">");
		y->tmp->next = init_data(y->ss, OUT);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (str[y->i] == '>' && str[y->i + 1] == '>')
	{
		y->ss = ft_strdup(">>");
		y->tmp->next = init_data(y->ss, APPEND);
		y->tmp = y->tmp->next;
		free(y->ss);
		y->i++;
	}
	else if (str[y->i] == '<' && str[y->i + 1] != '<')
	{
		y->ss = ft_strdup("<");
		y->tmp->next = init_data(y->ss, IN);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
}

void	lexer_2(char *str, t_stk *y)
{
	if (str[y->i] == '<' && str[y->i + 1] == '<')
	{
		y->ss = ft_strdup("<<");
		y->tmp->next = init_data(y->ss, HER);
		y->tmp = y->tmp->next;
		free(y->ss);
		y->i++;
	}
	if (str[y->i] == 39)
	{
		y->ss = ft_strdup("'");
		y->tmp->next = init_data(y->ss, SINGLE);
		y->tmp = y->tmp->next;
		free(y->ss);
	}
	else if (!check_is_oper(str[y->i]) && str[y->i] != ' ')
	{
		y->j = y->i;
		while(!check_is_oper(str[y->i]) && str[y->i] && str[y->i] != ' ')
			y->i++;
		y->ss = ft_substr(str, y->j, y->i - y->j);
		y->tmp->next = init_data(y->ss, WORD);
		y->tmp = y->tmp->next;
		free(y->ss);
		y->i--;
	}
}

void    lexer(char *str)
{
	t_stk y;

	y.i = 0;
	if (!str)
		return ;
	y.ss = ft_strdup("1");
	y.data_cmd = init_data(y.ss, 14);
	y.tmp = y.data_cmd;
	free(y.ss);
	while (str[y.i] == ' ' || str[y.i] == '\t')
		y.i++;
	y.b = ft_strlen(str);
	y.b--;
	while (str[y.b] == ' ' || str[y.b] == '\t')
		y.b--;
	while(str[y.i] && y.i <= y.b)
	{
		lexer_0(str, &y);
		lexer_1(str, &y);
		lexer_2(str, &y);
		y.i++;
	}
	y.i = 0;
	y.tmp = y.data_cmd;
	y.data_cmd = y.data_cmd->next;
	free(y.tmp->s);
	free(y.tmp);
	y.tmp = y.data_cmd;
    parser(y.data_cmd);
	while(y.tmp)
	{
		printf("%c || %d\n\n", y.tmp->type, y.i++);
		printf("%s\n", y.tmp->s);
		y.tmp = y.tmp->next;
	}
	fun_free(&y.data_cmd);
}
