/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:27:02 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/27 16:36:44 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

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

int	check_is_oper(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == 34 || c == 39)
		return (1);
	return (0);		
}

int	check_is_oper_error(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);		
}
int	checke_pipe(char *str)
{
	int	i;
	int	flg;

	i = 0;
	flg = 0;
	while(str[i])
	{
		if(str[i] == '|')
		{
			i++;
			while (str[i] == ' ')
				i++;
			if (str[i] == '\0')
				return (1);
		}
		else
			i++;
	}
	return (flg);
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
int	check_error(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '>' || str[i] == '<' || str[i] == '|')
		i++;
	if (str[i] == '\0')
	{
		printf("Minishell$: syntax error !\n");
		return (1);
	}
	else
		i = 0;
	while(str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '|')
	{
		printf("Minishell$: syntax error near unexpected token `|'\n");
		return (1);
	}
	while (str[i] == '>' || str[i] == '<')
		i++;
	while(str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '\0')
	{
		printf("Minishell$: syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0);
}

void signal_handler(int signal) {
    // readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	if (signal == SIGQUIT)
		exit(0);
    // exit(1);
	// return ;
}

int main(int ac, char **av, char **env)
{
	char *str;
	int	flg_d;
	int	flg_s;
	int	error;
	t_env *envs;

	(void )ac;
	(void )av[0];
	envs = creat_env_list(env);
	signal(SIGINT, signal_handler);
	str = readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	while(1)
	{
		error = check_error(str);
		while (1 && !error)
		{
			flg_d = checke_pipe(str);
			if (flg_d)
				str = ft_strjoin(str, readline("pipe> "));
			else
				break;
		}
		while (1 && !error)
		{
			flg_d = checke_double(str);
			flg_s = checke_single(str);
			if (flg_d % 2 != 0 || flg_s % 2 != 0)
				str = ft_strjoin(str, readline("quote> "));
			else
				break;
		}
		if (ft_strnstr(str, "clear", 6))
		{
			printf("\033[2J");
			printf("\033[1;1H");
		}
		else if(str && !error)
		{
			add_history(str);
			lexer(str, envs);
		}
		free(str);
		// rl_on_new_line();
		//  rl_replace_line("Enter something else: ", 0);
		// rl_redisplay();
		str = readline("\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	}
	fun_free_env(&envs);
}

// < j| ls seg fault