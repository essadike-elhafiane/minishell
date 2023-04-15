/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:06:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/15 00:40:12 by eelhafia         ###   ########.fr       */
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
	while(env[i])
	{
		if(ft_strnstr(env[i], ss, len))
		{
			if(env[i][len] == '=')
				s = ft_substr(env[i]+ len, len, ft_strlen(env[i]));
				// printf("%s\n", env[i]);
		}
		i++;
	}
	return (s);
}

void    parser(t_shell *data, char **env)
{
	t_stk	y;
	t_shell	*tmp;

	y.i = 0;
	y.j = 0;
	tmp = data;
	while(tmp)
	{
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
