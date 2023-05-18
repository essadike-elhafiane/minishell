/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 02:18:09 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/14 22:56:20 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_double_oper_h_p(t_stk *y, t_shell *tmp)
{
	if (tmp->type == PIPE)
	{
		y->i++;
		if (y->i > 1)
		{
			ft_putstr_fd("➜  Minishell$: syntax error\n", 2);
			g_status = 258;
			return (1);
		}
	}
	else
		y->i = 0;
	return (0);
}

int	check_double_oper_h(t_stk *y, t_shell *tmp)
{
	if (tmp->type == IN
		|| tmp->type == OUT || tmp->type == HER || tmp->type == APPEND)
	{
		y->j++;
		if ((tmp->next && tmp->next->type != WORD && tmp->next->type != DOUBLE
				&& tmp->next->type != SINGLE && tmp->next->type != WSPACE
				&& tmp->next->type != PIPE) || !tmp->next)
		{
			if (tmp->type == '>' && tmp->next->type == PIPE)
				y->i = y->i;
			else
			{
				ft_putstr_fd("➜  Minishell$: syntax error\n", 2);
				return (1);
			}
		}
		if (y->j > 1)
		{
			ft_putstr_fd("➜  Minishell$: syntax error\n", 2);
			return (1);
		}
	}
	else
		y->j = 0;
	return (0);
}

int	check_double_oper(t_shell *data)
{
	t_shell	*tmp;
	t_stk	y;

	tmp = data;
	y.i = 0;
	y.j = 0;
	while (tmp)
	{
		if (tmp->type == WSPACE)
			tmp = tmp->next;
		if (tmp)
		{
			if (check_double_oper_h_p(&y, tmp))
				return (1);
			if (check_double_oper_h(&y, tmp))
			{
				g_status = 258;
				return (1);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

int	check_is_word_after_oper(t_shell *data)
{
	t_shell	*tmp;
	t_stk	y;

	tmp = data;
	y.i = 0;
	y.j = 0;
	while (tmp)
	{
		if (tmp->type == IN || tmp->type == OUT
			|| tmp->type == HER || tmp->type == APPEND)
		{
			tmp = tmp->next;
			if (tmp && tmp->type == WSPACE)
				tmp = tmp->next;
			if (!tmp || (tmp->type != WORD
					&& tmp->type != DOUBLE && tmp->type != SINGLE))
			{
				printf("➜ Minishell$: tax error !\n");
				return (1);
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (0);
}

char	*expand(char *ss, t_env *env)
{
	char	*s;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(ss);
	s = NULL;
	while (env)
	{
		if (ft_strnstr(env->env, ss, len))
		{
			if (env->env[len] == '=')
				s = ft_strdup(env->env + len +1);
		}
		env = env->next;
	}
	free(ss);
	ss = NULL;
	return (s);
}
