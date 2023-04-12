/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:27:02 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/12 18:18:12 by eelhafia         ###   ########.fr       */
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
// int	is_caracter(char c)
// {
// 	if (c >= )
// }
// t_shell	*init_list(char *ss, t_shell **data_cmd)
// {
// 	static t_shell **tmp;

	
// 	if ((*data_cmd) == NULL)
// 		(*data_cmd) = init_data(ss, PIPE);
// 	else
// 	{
// 		(*data_cmd)->next = init_data(ss, PIPE);
// 		printf("fghj\n");
// 		// tmp =(t_shell *) ft_lstlast((t_list *)(*data_cmd));
// 		// (*tmp)->next = init_data(ss, PIPE);
// 		(*data_cmd) = (*data_cmd)->next;
// 	}
// 	free(ss);
// 	return (tmp);
// }
int	check_is_oper(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == 32 || c == 34 || c == 39 || c == '\t')
		return (1);
	return (0);		
}

void    lexer(char *str)
{
	int     i;
	int		j;
	int		b;
	t_shell *data_cmd;
	t_shell *tmp;
	char *ss;

	i = 0;
	ss = NULL;
	data_cmd = malloc(sizeof(t_shell));
	data_cmd->type = 14;
	data_cmd->s = NULL;
	tmp = data_cmd;
	if (!str)
		return ;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	b = ft_strlen(str);
	b--;
	while (str[b] == ' ' || str[b] == '\t')
		b--;
	while(str[i] && i <= b)
	{
		if (str[i] == ' ')
			i++;
		if (str[i] == '|')
		{
			ss = ft_strdup("|");
			tmp->next = init_data(ss, PIPE);
			tmp = tmp->next;
			free(ss);
		}
		else if (str[i] == 34)
		{
			ss = ft_strdup("\34");
			tmp->next = init_data(ss, DOUBLE);
			tmp = tmp->next;
			free(ss);
		}
		else if (str[i] == '>' && str[i + 1] != '>')
		{
			ss = ft_strdup(">");
			tmp->next = init_data(ss, OUT);
			tmp = tmp->next;
			free(ss);
		}
		else if (str[i] == '>' && str[i + 1] == '>')
		{
			ss = ft_strdup(">>");
			tmp->next = init_data(ss, APPEND);
			tmp = tmp->next;
			free(ss);
			i++;
		}
		else if (str[i] == '<' && str[i + 1] != '<')
		{
			ss = ft_strdup("<");
			tmp->next = init_data(ss, IN);
			tmp = tmp->next;
			free(ss);
		}
		else if (str[i] == '<' && str[i + 1] == '<')
		{
			ss = ft_strdup("<<");
			tmp->next = init_data(ss, HER);
			tmp = tmp->next;
			free(ss);
			i++;
		}
		else if (str[i] == 39)
		{
			ss = ft_strdup("'");
			tmp->next = init_data(ss, SINGLE);
			tmp = tmp->next;
			free(ss);
		}
		else
		{
			while (str[i] == 32 && str[i])
				i++;
			j = i;
			while(!check_is_oper(str[i]) && str[i])
			{
				i++;
				if ((str[i] == '"' || str[i] == 39) && str[i +1] != ' ' && !check_is_oper(str[i +1]))
					i++;
			}
			ss = ft_substr(str, j, i - j);
			tmp->next = init_data(ss, WORD);
			tmp = tmp->next;
			free(ss);
			if (check_is_oper(str[i]))
				i--;
		}
		i++;
	}
	i = 0;
	data_cmd = data_cmd->next;
	while(data_cmd)
	{
		printf("%c || %d\n\n", data_cmd->type, i++);
		printf("%s\n", data_cmd->s);
		data_cmd = data_cmd->next;
	}
}

int main()
{
	char *str;
	// t_shell d;
	printf("\033[2J");
	printf("\033[1;1H");
	str = readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	while(str)
	{
		// d.type = WORD;
		// printf("%c\n\n", d.type);
		// printf("%s\n", str);
		lexer(str);
		str = readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	}
	
}