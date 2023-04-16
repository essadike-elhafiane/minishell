/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:07:00 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/16 22:38:13 by eelhafia         ###   ########.fr       */
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
		y->b = y->i;
		y->i++;
		while(str[y->i] != 34)
			y->i++;
		y->ss = ft_substr(str, y->b, y->i - y->b +1);
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
		y->b = y->i;
		y->i++;
		while(str[y->i] != 39)
			y->i++;
		y->ss = ft_substr(str, y->b, y->i - y->b +1);
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

t_env *init_env(char *env)
{
	t_env *data;

	data =(t_env *) malloc(sizeof(t_env));
	if(!data)
		exit(1);
	data->env = ft_strdup(env);
	data->next = NULL;
	return (data);
}

t_env *creat_env_list(char **env)
{
	int	i;
	t_env *env_l;
	t_env *tmp;
	
	if(!*env)
		return (NULL);
	env_l = init_env(env[0]);
	tmp = env_l;
	i = 1;
	while(env[i])
	{
		tmp->next = init_env(env[i]);
		tmp = tmp->next;
		i++;
	}
	return (env_l);
}

void    lexer(char *str, char **env)
{
	t_stk y;

	y.i = 0;
	if (!str || str[0] == '\0')
		return ;
	y.ss = ft_strdup("1");
	y.data_cmd = init_data(y.ss, 14);
	y.tmp = y.data_cmd;
	free(y.ss);
	while (str[y.i] == ' ' || str[y.i] == '\t')
		y.i++;
	while(str[y.i])
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
	y.data_cmd->env = creat_env_list(env);
    if (parser(y.data_cmd, y.data_cmd->env))
		return ;
	t_cmd *cmd;
	cmd = creat_cmd(y.data_cmd);
	while(cmd)
	{
		int i = 0;
		// printf("%c || %d\n\n", y.tmp->type, y.i++);
		while(cmd->cmd[i])
			printf("%s\n", cmd->cmd[i++]);
		printf("\n\n");
		cmd = cmd->next;
	}
	// while(y.tmp)
	// {
	// 	printf("%c || %d\n\n", y.tmp->type, y.i++);
	// 	printf("%s\n", y.tmp->s);
	// 	y.tmp = y.tmp->next;
	// }
	fun_free_env(&y.data_cmd->env);
	fun_free(&y.data_cmd);
}
