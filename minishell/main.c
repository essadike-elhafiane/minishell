/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:27:02 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/13 20:36:36 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell *init_data(char *ss, int type1)
{
	t_shell *data;

	data =(t_shell *) malloc(sizeof(t_shell));
	if(!data)
		exit(1);
	data->type = type1;
	data->s = ft_strdup(ss);
	data->next = NULL;
	return (data);
}

t_shell	*lstlast(t_shell *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	fun_free(t_shell **a)
{
	t_shell	*tmp;

	while (*a)
	{
		tmp = *a;
		*a = (*a)->next;
		free(tmp->s);
		free(tmp);
	}
	a = NULL;
}

int	check_is_oper(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == 34 || c == 39 || c == '$')
		return (1);
	return (0);		
}

int	checke_double(char *str)
{
	int	i;
	int	flg;

	i = 0;
	flg = 0;
	while(str[i])
	{
		if(str[i] == 34)
			flg++;
		i++;
	}
	return (flg);
}

int	checke_single(char *str)
{
	int	i;
	int	flg;

	i = 0;
	flg = 0;
	while(str[i])
	{
		if(str[i] == 39)
			flg++;
		i++;
	}
	return (flg);
}

int main()
{
	char *str;
	int	flg_d;
	int	flg_s;
	// t_shell d;
	printf("\033[2J");
	printf("\033[1;1H");
	str = readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	while(str)
	{
		// d.type = WORD;
		// printf("%c\n\n", d.type);
		// printf("%s\n", str);
		while (1)
		{
			flg_d = checke_double(str);
			flg_s = checke_single(str);
			if (flg_d % 2 != 0 || flg_s % 2 != 0)
				str = ft_strjoin(str, readline("> "));
			else
				break;
		}
		lexer(str);
		free(str);
		str = readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	}
}
