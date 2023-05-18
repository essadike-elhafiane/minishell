/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 21:27:02 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/18 19:24:59 by eelhafia         ###   ########.fr       */
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

void	loop_str(char *str, int error, t_env **envs)
{
	int	flg_d;
	int	flg_s;
	int		flg;

	flg = 0;
	while (!error)
	{
		flg_d = checke_double(str);
		flg_s = checke_single(str);
		if (flg_d % 2 != 0 || flg_s % 2 != 0)
			str = ft_strjoin(str, readline("quote> "));
		else
			break ;
		flg++;
		if (flg > 6)
			return (free(str), g_status = 1, ft_putstr_fd("error quotes !\n", 2));
	}
	if (word_stop(str, "clear"))
		printf("\033[2J\033[1;1H");
	if (ft_strnstr(str, "exit", 4) && (str[4] == '\0' || str[4] == ' '))
	{
		if (ft_exit(str))
			return (free(str));
	}
	else if (str && !error)
	{
		if (str[0] != '\0')
			add_history(str);
		lexer(str, envs);
	}
	free(str);
}

char	*check_pipe(int error, char *str)
{
	int		flg_d;

	while (!error)
	{
		flg_d = checke_pipe(str);
		if (flg_d)
			str = ft_strjoin(str, readline("pipe> "));
		else
			break ;
	}
	return (str);
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	int		error;
	t_env	*envs;

	(void )ac;
	(void )av[0];
	envs = creat_env_list(env);
	if (!envs)
		return (0);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	str = readline("➜  \033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	while (str)
	{
		error = check_error(str);
		str = check_pipe(error, str);
		loop_str(str, error, &envs);
		find_exit_status(envs);
		str = readline("➜  \033[1;36mMinishell\033[0m\033[0;35m$\033[0m ");
	}
	fun_free_env(&envs);
}
