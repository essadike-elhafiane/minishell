/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:06:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/30 14:53:25 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	check_double_oper(t_shell *data)
{
	t_shell *tmp;
	t_stk	y;

	tmp = data;
	y.i = 0;
	y.j = 0;
	while(tmp)
	{
		if (tmp->type == SPACE)
			tmp = tmp->next;
		if (tmp)
		{
			if (tmp->type == PIPE)
			{
				y.i++;
				if (y.i > 1)
				{
					printf("➜  Minishell$: syntax error near unexpected token `%s'\n", tmp->s);
					return (1);
				}
			}
			else
				y.i = 0;
			if (tmp->type == IN || tmp->type == OUT || tmp->type == HER || tmp->type == APPEND)
			{
				y.j++;
				if ((tmp->next && tmp->next->type != WORD && tmp->next->type != DOUBLE && tmp->next->type != SINGLE && tmp->next->type != SPACE && tmp->next->type != PIPE) || !tmp->next)
				{
					if (tmp->type == '>' && tmp->next->type == PIPE)
						y.i = y.i;
					else
					{
						printf("➜  Minishell$: syntax error near unexpected token `%s'\n", tmp->s);
						return (1);
					}
				}
				if (y.j > 1)
				{
					printf("➜  Minishell$: syntax error near unexpected token `%s'\n", tmp->s);
					return (1);
				}
			} 
			else
				y.j = 0;
			tmp = tmp->next;
		}
	}
	return (0);
}

int	check_is_word_after_oper(t_shell *data)
{
	t_shell *tmp;
	t_stk	y;

	tmp = data;
	y.i = 0;
	y.j = 0;
	while(tmp)
	{
		if  (tmp->type == IN ||tmp->type == OUT || tmp->type == HER || tmp->type == APPEND)
		{
			tmp = tmp->next;
			if  (tmp && tmp->type == SPACE)
				tmp = tmp->next;
			if(!tmp || (tmp->type != WORD && tmp->type != DOUBLE && tmp->type != SINGLE))
			{
				printf("➜ Minishell$: syntax error !\n");
				return (1);
			}
		}
		if(tmp)
			tmp = tmp->next;
	}
	return (0);
}

void	check_expand(t_stk *y, t_shell *tmp, t_env *env)
{
	int flg = 0;
	y->i = 0;
	if (tmp->type == SINGLE)
		return ;
	while (tmp && tmp->s[y->i])
	{
		if (tmp->s[y->i] == '$' && (tmp->s[y->i + 1] == '$'))
			y->i++;
		if (tmp->s[y->i] == '$' && tmp->s[y->i +1] != '\0'
			&& (tmp->s[y->i + 1] != '$' && tmp->s[y->i + 1] != ' '))
		{
			y->i++;
			y->b = y->i;
			while (ft_isalpha(tmp->s[y->i]) || ft_isdigit(tmp->s[y->i]) || tmp->s[y->i] == '_')
				y->i++;
			y->ss = ft_substr(tmp->s, y->b, y->i - y->b);
			y->ss = expand(y->ss, env);
			y->back = ft_strdup(tmp->s + y->i);
			if (!y->ss && tmp->s[y->i] && y->back)
				y->i = 0;
			y->front = ft_substr(tmp->s, 0, y->b -1);
			free(tmp->s);
			if (y->front)
				tmp->s = ft_strjoin(y->front, y->ss);
			tmp->s = ft_strjoin(tmp->s, y->back);
			flg = 1;
		}
		else
			y->i++;
	}
	if (flg && tmp->s && ft_strchr(tmp->s, ' '))
	{
		t_shell *tmp2;
		char **spl;
		int i;

		i = 1;
		tmp2 = tmp->next;
		spl = ft_split(tmp->s, ' ');
		tmp->s = spl[0];
		if (spl[i])
		{
			tmp->next = malloc(sizeof(t_shell));
			tmp = tmp->next;
			tmp->s = ft_strdup(" ");
			tmp->type = SPACE;
			tmp->next = NULL;
		}
		while (spl[i])
		{
			tmp->next = malloc(sizeof(t_shell));
			tmp = tmp->next;
			tmp->s = spl[i];
			tmp->type = WORD;
			tmp->next = NULL;
			i++;
			if (spl[i])
			{
				tmp->next = malloc(sizeof(t_shell));
				tmp = tmp->next;
				tmp->s = ft_strdup(" ");
				tmp->type = SPACE;
				tmp->next = NULL;
			}
		}
		tmp->next = tmp2;
	}
}

	// if (flg && tmp->s && ft_strchr(tmp->s, ' '))
	// {
	// 	t_shell *tmp2;
	// 	char **spl;
	// 	int i;

	// 	i = 1;
	// 	tmp2 = tmp->next;
	// 	spl = ft_split(tmp->s, ' ');
	// 	tmp->s = spl[0];
	// 	if (spl[i])
	// 	{
	// 		tmp->next = malloc(sizeof(t_shell));
	// 		tmp = tmp->next;
	// 		tmp->s = ft_strdup(" ");
	// 		tmp->type = SPACE;
	// 		tmp->next = NULL;
	// 	}
	// 	while (spl[i])
	// 	{
	// 		tmp->next = malloc(sizeof(t_shell));
	// 		tmp = tmp->next;
	// 		tmp->s = spl[i];
	// 		printf("%s\n", tmp->s);
	// 		tmp->type = WORD;
	// 		tmp->next = NULL;
	// 		i++;
	// 		if (spl[i])
	// 		{
	// 			tmp->next = malloc(sizeof(t_shell));
	// 			tmp = tmp->next;
	// 			tmp->s = ft_strdup(" ");
	// 			tmp->type = SPACE;
	// 			tmp->next = NULL;
	// 		}
	// 	}
	// 	tmp->next = tmp2;
	// }

int    parser(t_shell *data, t_env *env)
{
	t_stk	y;
	t_shell	*tmp;

	tmp = data;
	while(tmp)
	{
		
		y.i = 0;
		if (tmp->type == HER)
		{
			tmp = tmp->next;
			if (tmp && tmp->type == SPACE)
				tmp = tmp->next;
			while (tmp && !check_is_token(tmp->type))
				tmp = tmp->next;
			// printf("%s\n", tmp->s);
		}
		if (tmp && tmp->s)
			check_expand(&y, tmp, env);
		if (tmp)
			tmp = tmp->next;
	}
	if (check_double_oper(data))
		return (1);
	if (check_is_word_after_oper(data))
		return (1);
	return (0);
}


	// if(y.i % 2 != 0 || y.j % 2 != 0)
	// {
	// 	printf("minishell: syntax error \n");
	// 	return ;
	// 	printf("%s", y.ss);
	// }
// 
