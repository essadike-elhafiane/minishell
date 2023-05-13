/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:27:02 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/13 22:33:15 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"



t_shell	*init_data(char *ss, int type1)
{
	t_shell	*data;

	data = (t_shell *) malloc(sizeof(t_shell));
	if (!data)
		exit(1);
	data->type = type1;
	data->var_re = 0;
	data->len_spl = 0;
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
int check_error_help(char *str, int i)
{
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '|')
	{
		printf("Minishell$: syntax error near unexpected token `|'\n");
		status = 258;
		return (1);
	}
	while (str[i] == '>' || str[i] == '<')
		i++;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '\0')
	{
		printf("Minishell$: syntax error near unexpected token `newline'\n");
		status = 258;
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
		printf("Minishell$: syntax error !\n");
		status = 258;
		return (1);
	}
	else
		i = 0;
	if (check_error_help(str, i))
		return (1);
	return (0);
}

void signal_handler(int signal) 
{
   if (signal == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG))
		{
			printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			status = 1;
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
		status = ft_atoi(s + 4);
	exit(status);
}

void	loop_str(char *str, int error, t_env **envs)
{
	int	flg_d;
	int	flg_s;

	while (!error)
	{
		flg_d = checke_double(str);
		flg_s = checke_single(str);
		if (flg_d % 2 != 0 || flg_s % 2 != 0)
			str = ft_strjoin(str, readline("quote> "));
		else
			break ;
	}
	if (word_stop(str, "clear"))
		printf("\033[2J\033[1;1H");
	if (ft_strnstr(str, "exit", 4) && (str[4] == '\0' || str[4] == ' '))
		ft_exit(str);
	else if (str && !error)
	{
		if (str[0] != '\0')
			add_history(str);
		lexer(str, envs);
	}
	free(str);
}

void	find_exit_status(t_env *env)
{
	while (env)
	{
		if(strnstr(env->env, "?", 2))
		{
			free(env->env);
			env->env = ft_strjoin(ft_strdup("?="), ft_itoa(status));
			break;
		}
		env = env->next;
	}
}

void h(void)
{
	system("leaks minishell");
}
int	main(int ac, char **av, char **env)
{
	char	*str;
	int		error;
	int		flg_d;
	t_env	*envs;

	(void )ac;
	(void )av[0];
	envs = creat_env_list(env);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	str = readline(
			"\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	while (str)
	{
		error = check_error(str);
		while (!error)
		{
			flg_d = checke_pipe(str);
			if (flg_d)
				str = ft_strjoin(str, readline("pipe> "));
			else
				break ;
		}
		loop_str(str, error, &envs);
		find_exit_status(envs);
		str = readline(
				"\033[1;32m➜  \033[0m\033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	}
	fun_free_env(&envs);
}

//>""
// < j| ls seg fault

// <inp >out :inp file doesnt exist

// always print error in stderr not stdout
// echo $_ : handle _ in variables names and number shouldnt be in first char in var name
// echo $.
// <ok <<ok
// $$$USER
// < ss | ls
// lsof -c minishell
// hmeftah
// int main(int ac, char **av)
// {
	
// }
// multi space not error
// when creat a $? in first time and when run minishell inside of minishell the program the $? and creat another $? so thats given too $? in env