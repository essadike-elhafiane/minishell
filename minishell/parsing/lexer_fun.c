/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_fun.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:07:00 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/14 01:29:31 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*init_env(char *env)
{
	t_env	*data;

	data = (t_env *) malloc(sizeof(t_env));
	if (!data)
		exit(1);
	data->env = ft_strdup(env);
	data->p = 1;
	data->next = NULL;
	return (data);
}

void	lexer_help(t_stk *y, t_env **envs)
{
	t_cmd	*cmd;

	y->i = 0;
	y->tmp = y->data_cmd;
	y->data_cmd = y->data_cmd->next;
	free(y->tmp->s);
	free(y->tmp);
	y->tmp = y->data_cmd;
	y->data_cmd->env = envs;
	if (parser(y->data_cmd, *(y->data_cmd->env)))
		return (fun_free(&y->data_cmd));
	y->tmp = y->data_cmd;
	cmd = creat_cmd(y->data_cmd);
	if (!cmd)
		return (fun_free(&y->data_cmd));
	cmd->env = y->data_cmd->env;
	// t_cmd *tmpp = cmd;
	// int i;
	// while(tmpp)
	// {	i = 0;
	// 	while (tmpp->cmd[i])
	// 	printf("%s\n", tmpp->cmd[i++]);
	// 	tmpp = tmpp->next;
	// }
	exection(cmd);
	fun_free_cmd(&cmd);
	fun_free(&y->data_cmd);
}

t_env	*creat_env_list(char **env)
{
	int		i;
	int		flg;
	t_env	*env_l;
	t_env	*tmp;

	flg = 0;
	if (!*env)
	{
		env_l = init_env("");
		env_l->p = 0;
		return (env_l);
	}
	env_l = init_env(env[0]);
	tmp = env_l;
	i = 0;
	while (env[++i])
	{
		tmp->next = init_env(env[i]);
		if (ft_strnstr(env[i], "?", 1))
			flg = 1;
		tmp = tmp->next;
	}
	if (!flg)
		tmp->next = init_env("?=0");
	return (env_l);
}
