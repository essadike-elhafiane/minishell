/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:06:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/15 01:40:41 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand(char *ss, char **env)
{
	char *s;
	int	i;
	int	len;
	
	i = 0;
	len = ft_strlen(ss);
	s = NULL;
	while(env[i])
	{
		if(ft_strnstr(env[i], ss, len))
		{
			if(env[i][len] == '=')
				s = ft_strdup(env[i] + len +1);
		}
		i++;
	}
	free(ss);
	ss = NULL;
	// printf("%s\n", s);
	return (s);
}

void    parser(t_shell *data, char **env)
{
	t_stk	y;
	t_shell	*tmp;

	tmp = data;
	while(tmp)
	{
		
		y.i = 0;
		while(tmp->s[y.i])
		{
			if (tmp->s[y.i] == '$')
			{
				y.i++;
				y.b = y.i;
				while((ft_isalpha(tmp->s[y.i]) || ft_isdigit(tmp->s[y.i])))
					y.i++;
				y.ss = ft_substr(tmp->s, y.b, y.i - y.b);
				y.ss = expand(y.ss, env);
				
				// printf("%s\n", y.ss);
				// exit(1);
				y.back = ft_strdup(tmp->s + y.i);
				y.front = ft_substr(tmp->s, 0, y.b -1);
				free(tmp->s);
				if (y.front)
					tmp->s = ft_strjoin(y.front, y.ss);
				tmp->s = ft_strjoin(tmp->s, y.back);
			}
			y.i++;
		}
		tmp = tmp->next;
	}
}
	// if(y.i % 2 != 0 || y.j % 2 != 0)
	// {
	// 	printf("minishell: syntax error \n");
	// 	return ;
	// 	printf("%s", y.ss);
	// }
