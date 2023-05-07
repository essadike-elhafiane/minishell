/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:07:00 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/08 00:45:57 by eelhafia         ###   ########.fr       */
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
		y->i++;
		y->b = y->i;
		while(str[y->i] != 39)
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

t_env	*init_env(char *env)
{
	t_env	*data;

	data = (t_env *) malloc(sizeof(t_env));
	if (!data)
		exit(1);
	data->env = ft_strdup(env);
	data->next = NULL;
	return (data);
}

t_env	*creat_env_list(char **env)
{
	int		i;
	t_env	*env_l;
	t_env	*tmp;
	
	if (!*env)
		return (NULL);
	env_l = init_env(env[0]);
	tmp = env_l;
	i = 1;
	while (env[i])
	{
		tmp->next = init_env(env[i]);
		tmp = tmp->next;
		i++;
	}
	env_l->size = i - 2;
	return (env_l);
}

void	lexer(char *str, t_env *envs)
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
	// if (!env_creat)
	// {
	y.data_cmd->env = envs;
	// 	env_creat++;
	// }
	// while(y.tmp && y.tmp->s)
	// {
	// 	printf("%s type %c\n", y.tmp->s, y.tmp->type);
	// 	y.tmp = y.tmp->next;
	// }
	// // exit(1);
    if (parser(y.data_cmd, y.data_cmd->env))
		return ;
	t_cmd *cmd;
	y.tmp = y.data_cmd;
	t_cmd *cmdd;
	cmd = creat_cmd(y.data_cmd);
	if (!cmd)
		return ;
	// exit(1);
	// clean_cmd(cmd);
	cmd->env = y.data_cmd->env;
	// printf("asafa\n");
	// cmdd = cmd;
	// while(cmdd)
	// {
	// 	int i = 0;
		
	// 	// printf("%c || %d\n\n", y.tmp->type, y.i++);
	// 	if (cmdd->cmd)
	// 	{
	// 		while(cmdd->cmd[i])
	// 		{
	// 			printf("%d\n", cmdd->fd_input);
	// 			printf("%s\n", cmdd->cmd[i++]);
	// 		}
	// 	}
	// 	cmdd = cmdd->next;
	// }
	exection(cmd);
	// while(y.tmp)
	// {
	// 	printf("%c || %d\n\n", y.tmp->type, y.i++);
	// 	printf("%s\n", y.tmp->s);
	// 	y.tmp = y.tmp->next;
	// }
	fun_free_cmd(&cmd);
	fun_free(&y.data_cmd);
}
