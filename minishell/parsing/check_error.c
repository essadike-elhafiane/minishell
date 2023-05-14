/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 23:29:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/14 22:59:17 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_error_help(char *str, int i)
{
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '|')
	{
		ft_putstr_fd("Minishell$: syntax error near unexpected token `|'\n", 2);
		g_status = 258;
		return (1);
	}
	while (str[i] == '>' || str[i] == '<')
		i++;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '\0')
	{
		ft_putstr_fd(
			"Minishell$: syntax error near unexpected token `newline'\n", 2);
		g_status = 258;
		return (1);
	}
	return (0);
}

int	check_error(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i] == ' ')
		i++;
	if (str[i] == '\0')
		return (1);
	while (str[i] == ' ' || str[i] == '\t'
		|| str[i] == '>' || str[i] == '<' || str[i] == '|')
		i++;
	if (str[i] == '\0')
	{
		ft_putstr_fd("Minishell$: syntax error !\n", 2);
		g_status = 258;
		return (1);
	}
	else
		i = 0;
	if (check_error_help(str, i))
		return (1);
	return (0);
}

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG))
		{
			printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			g_status = 1;
		}
	}
}

void	ft_exit(char *s)
{
	int	i;

	i = 5;
	printf("exit\n");
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
		{
			printf("minishell: exit:  %s: numeric argument required\n", s +4);
			exit(255);
		}
		i++;
	}
	if (!s[i] && i > 5)
		g_status = ft_atoi(s + 4);
	exit(g_status);
}

void	find_exit_status(t_env *env)
{
	while (env)
	{
		if (ft_strnstr(env->env, "?", 2))
		{
			free(env->env);
			env->env = ft_strjoin(ft_strdup("?="), ft_itoa(g_status));
			break ;
		}
		env = env->next;
	}
}
