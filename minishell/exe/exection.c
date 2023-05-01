/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/01 01:02:21 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error_message(char *str)
{
	write(2, str, ft_strlen(str));
	exit(1);
}

void	ft_command_path(t_cmd *cmd , char **path)
{
	int	i;
	int j;
	char *tmp;
	j = 0;

	while(cmd->cmd[j])
	{
		i = 0;
		if (ft_strchr(cmd->cmd[j] ,'/') && !access(cmd->cmd[j], X_OK))
		{
			cmd->cmd_path = cmd->cmd[j];
				return;
		}
		while (path[i])
		{
			tmp = ft_strjoin_no_free(path[i], "/");
			cmd->cmd_path = ft_strjoin_no_free(tmp, cmd->cmd[j]);
			free(tmp);
			if (!access(cmd->cmd_path, X_OK))
				return ;
			free(cmd->cmd_path);
			cmd->cmd_path = NULL;
			i++;
		}
		j++;
	}
	error_message("➜ Minishell$: command not found\n");
}



char	**get_path(t_env *env)
{
	int	i;

	i = -1;
	while (env)
	{
		if (ft_strnstr(env->env, "PATH", 6))
			return (ft_split(env->env +5, ':'));
		env = env->next;
	}
	return (NULL);
}

void creat_files(t_cmd *cmd, t_cmd *tmp2, int i)
{
	if(cmd->fd_out == 1 && cmd->next)
		cmd->fd_out = cmd->fd[1];
	if(cmd->fd_input == 0 && i != 0)
		cmd->fd_input = tmp2->fd[0];
}

// void cmd_pwd(t_cmd *cmd)
// {
// 	int i;

// 	i = 0;
// 	while(cmd->enve[i])
// 	{
// 		if(ft_strncmp(cmd->enve[i++], "pwd" ,5))
// 			printf("%s", cmd->enve[i] + 4);
// 	}	
// }
void	cmd_echo(t_cmd *cmd)
{
	int		flg_n;
	t_stk	y;

	y.i = 1;
	if (word_stop("echo", cmd->cmd[0]))
	{
		flg_n = 0;
		while (cmd->cmd[y.i])
		{
			y.j = 0;
			while (cmd->cmd[y.i][y.j] && cmd->cmd[y.i][0] == '-')
			{
				y.j++;
				while (cmd->cmd[y.i][y.j] == 'n' && cmd->cmd[y.i][y.j])
					y.j++;
				if (!cmd->cmd[y.i][y.j])
				{	
					flg_n = 1;
					y.i++;
				}
				else 
					break;
				y.j = 0;
			}
			while (cmd->cmd[y.i])
			{
				ft_putstr_fd(cmd->cmd[y.i], 1);
				y.i++;
				if (cmd->cmd[y.i])
					write(1, " ", 1);
			}
		}
	}
	if (!flg_n)
		printf("\n");
	cmd = cmd->next;
	close(cmd->fd_input);
	close(cmd->fd_out);
	exit(0);
}

void cmd_pwd(t_cmd *cmd)
{
	char cwd[1024];

	printf("%s\n",getcwd(cwd, sizeof(cwd)));
	exit(0);
}

void export_sort(t_export *export)
{
	t_export *tmp;
	t_export *tmp2;
	char *swap;

	tmp = export;
	tmp2 = export;
	while(tmp)
	{
		tmp2 = tmp->next;
		while(tmp2)
		{
			if((ft_strncmp(tmp->export, tmp2->export, ft_strlen(tmp->export) + 2)) > 0)
			{
				swap = tmp->export;
				tmp->export = tmp2->export;
				tmp2->export = swap;
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
}

t_export *creat_export(t_env *envs)
{
	t_export *export;
	t_export *tmp;

	export = malloc(sizeof(t_export*));
	export->export = NULL;
	export->size=envs->size;
	export->next = NULL;
	if(envs->env)
		export->export = envs->env;
	tmp = export;
	export->head = export;
	envs = envs->next;
	while(envs)
	{
		tmp->next =(t_export*) malloc(sizeof(t_export));
		tmp = tmp->next;
		tmp->head = export;
		tmp->next = NULL;
		tmp->export = ft_strdup(envs->env);
		envs = envs->next;
	}
	export_sort(export);
	return (export);
}
// void	cmd_unset(t_env **envs, t_cmd *cmd)
// {
	
// }


int ft_strcchr(char *str, char c)
{
	int i;

	i =0;
	while(str[i])
	{
		if(str[i] == c)
			return(i);
		i++;
	}
	return(-1);
}


int	string_find(char *word, char *str)
{
	int	i;
	int j;
	i = 0;
	
	j = ft_strlen(word);
	while(word[i] == str[i] && i < j)
	{
		if(word[i] != str[i])
			return (0);
		i++;
		if(i == j - 1)
		{	
			return 1;
		}
	}
	return (0);
}

// void check_export(t_export *tmp, t_cmd *cmd)
// {
// 	int i;

// 	i = 0;
// 	while(tmp)
// 	{
// 		if((i = ft_strcchr(tmp->export ,'=')) != - 1 && ft_strnstr(tmp->export,cmd->cmd[1] ,i))
// 		{
			
// 		}
// 		else if(i = -1 && string_find(tmp->export,cmd->cmd[1]))
// 		{

// 		}
// 		tmp = tmp->next;
// 	}
// }


#include <string.h>
int  check_export(t_env *envs, t_cmd *cmd)
{
	t_export *tmp;
	int i;
	int a;
	int flg;
	int b;
	char *s;

	a = 1;
	while(cmd && cmd->cmd[a])
	{
		flg = 0;
		i = -1;
		b = 0;
		tmp = envs->export;
		i = ft_strcchr(cmd->cmd[a] ,'=');
		s = ft_substr(cmd->cmd[a], 0, i);
		if (!cmd->cmd[a + 1] && cmd->cmd[a][0] == '\0')
			return (1);
		while (cmd->cmd[a] && cmd->cmd[a][b] && cmd->cmd[a][b] != '=')
		{
			if (ft_isdigit(cmd->cmd[a][0]) || (!ft_isalpha(cmd->cmd[a][b]) && !ft_isdigit(cmd->cmd[a][b] && cmd->cmd[a][b] != '_' )))
			{
				ft_putstr_fd("minishell: not a valid identifier", 2);
				b = -1;
				break;
			}
			b++;
		}
		while(tmp && cmd->cmd[a] && b > -1)
		{
			if ( i > 0 && strnstr(tmp->export, s, ft_strlen(s)))
			{
				// if (tmp->export[ft_strlen(s) -1] != '\0' && tmp->export[ft_strlen(s) -1] != '=')
				// 	break;
				free(tmp->export);
				tmp->export = ft_strdup(cmd->cmd[a]);
				flg = 1;
			}
			else if (ft_strnstr(tmp->export, cmd->cmd[a], ft_strlen(cmd->cmd[a])))
			{
				if (tmp->export[ft_strlen(s)] != '\0' && tmp->export[ft_strlen(s)] != '=')
					break;
				flg = 1;
			}
			tmp = tmp->next;
		}
		free(s);
		if (!flg && b > -1)
			cmd_export(cmd->env, cmd->cmd[a]);
		a++;
	}
	return (0); 
}


// int  check_exp_sp(t_env *envs, t_cmd *cmd, char c)
// {
// 	t_export *tmp;

// 	tmp = envs->export;
// 	while(envs)
// 	{
// 		if ((ft_strcchr(cmd->cmd[1] ,'=') == -1  && string_find(cmd->cmd[1][1] , tmp->export)))
// 			return (1);
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

void	cmd_export_fork(t_env *envs, t_cmd *cmd)
{
		t_export *tmp;
	t_env		*tmp2;
	int i;
	
	i = 0;
	tmp = envs->export;
	tmp2 = envs;
	// printf("if\n");
	// if(cmd->cmd[1] == NULL || cmd->cmd[2] == NULL)
	// {
		while(tmp)
		{
			printf("declare -x %s\n", tmp->export);
			tmp = tmp->next;
		}
		exit(0);
	// }
	// printf("ERROR\n");
	// exit(0);
}

void	cmd_export(t_env *envs, char *cmd)
{
	t_export *tmp;
	t_env		*tmp2;

	tmp = envs->export;
	tmp2 = envs;
	// printf("if\n");
	
		// printf("else\n");
		// check_export_carataire(cmd->cmd[1]);	
	// if (ft_isalpha(cmd->cmd[1][y->i]) || ft_isdigitcmd->cmd[1][y->i]) || cmd->cmd[1][y->i] == '_')
	
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = (t_export *) malloc(sizeof(t_export));
		tmp = tmp->next;
		tmp->next = NULL;
		tmp->export = ft_strdup(cmd);
		// printf("%s\n",tmp->export);
		if(ft_strchr(cmd, '='))
		{
			while(tmp2->next)
				tmp2 = tmp2->next;
			tmp2->next = (t_env *) malloc(sizeof(t_env));
			tmp2 = tmp2->next;
			tmp2->next = NULL;
			tmp2->env = ft_strdup(cmd);
		}
		export_sort(envs->export);
}

void	cmd_env(t_env *envs)
{
	t_env *tmp;

	tmp = envs;
		while(tmp)
		{
			printf("%s\n", tmp->env);
			tmp = tmp->next;
		}
		
	exit(0);
}


char **envs_tab(t_env *envs)
{
	char **tab;
	t_env *tmp;
	int i;

	i = 1;
	tmp = envs;
	while(tmp)
	{
		i++;	
		tmp = tmp->next;
	}
	tab = (char **)malloc(sizeof(char *) * i);
	if(!tab)
	{
		printf("error\n");
		exit(0);
	}
	tmp = envs;
	i = 0;
	while(tmp)
	{
		tab[i] = ft_strdup(tmp->env);
		tmp = tmp->next;
		i++;
	}
	tab[i] = NULL;
	return(tab);
}


void	ft_command(t_cmd *cmd)
{	
	char **tab;

	if(cmd->fd_input != 0)
		dup2(cmd->fd_input , STDIN_FILENO);
	if(cmd->fd_out != 1)	
		dup2(cmd->fd_out , STDOUT_FILENO);
	if (cmd->next)
		close(cmd->fd[1]);
	if(cmd->fd_input != 0)
		close(cmd->fd_input);
	if(cmd->fd_out != 1)
		close(cmd->fd_out);
	if (ft_strnstr(cmd->cmd[0], "echo", 5))
		cmd_echo(cmd);
	else if(ft_strnstr (cmd->cmd[0], "pwd", 4))
		cmd_pwd(cmd);
	else if(ft_strnstr (cmd->cmd[0], "export", 7))
		cmd_export_fork(cmd->env, cmd);
	else if(ft_strnstr (cmd->cmd[0], "env", 7))
		cmd_env(cmd->env);
	else
	{
		ft_command_path(cmd ,cmd->paths);
		tab = envs_tab(cmd->env);
		execve(cmd->cmd_path, cmd->cmd, tab);
		perror("execve");
	}
}

// !check_export(cmd->env , cmd))
int is_not_fork(t_cmd *cmd)
{
	
	if (cmd->cmd && ft_strnstr(cmd->cmd[0], "export", 7))
	{
		// printf("fff\n");
		if (cmd->cmd[1] == NULL)
			return (0); 
		else
		{
			// printf("fff2\n");
			if (check_export(cmd->env , cmd))
				return (0);
			return (1);
		}
	}
	else if (cmd->cmd && ft_strnstr (cmd->cmd[0], "unset", 7) && cmd->cmd[1] != NULL)
		return (1);
	return (0);
}

void ft_pipe(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_stk	y;
	int input;
	int out;
	t_cmd *tmp2;
	tmp2 = NULL;
	y.i = 0;
	tmp = cmd;
	while(tmp)
	{
		tmp =tmp->next;
		y.i += 1;	
	}
	// printf("%d\n",y.i);
	// printf("gg\n");
	y.i = 0;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->next)
		{
			// printf("1  %d\n", y.i);
			pipe(tmp->fd);
			// printf("3333333\n");
		}
		// ft_putstr_fd("error\n", 2);
		if (is_not_fork(tmp))
		{
			// ft_putstr_fd("error12\n", 2);
			// printf("2  %d\n", y.i);
			creat_files(tmp ,tmp2, y.i);
			if(cmd->fd_input == -1)
				return;
			if(cmd->fd_input != 0)
			{
				// input = dup(STDIN_FILENO);
				// dup2(cmd->fd_input , STDIN_FILENO);
				close(cmd->fd_input);
			}
			if (cmd->fd_out != 1)
			{	
				out = dup(STDOUT_FILENO);	
				dup2(cmd->fd_out , STDOUT_FILENO);
			}
			if (cmd->next)
				close(cmd->fd[1]);
			// if (cmd->fd_input != 0)
			// 	close(cmd->fd_input);
			if (cmd->fd_out != 1)
				close(cmd->fd_out);
			// if (ft_strnstr (cmd->cmd[0], "export", 7))
				
			// else if (ft_strnstr (cmd->cmd[0], "unset", 6))
			// 	cmd_unset(cmd->env, cmd);
			// if(cmd->fd_input != 0)
			// {	
			// 	dup2(input , STDIN_FILENO);
			// 	close(input);	
			// }
			if(cmd->fd_out != 1)
			{	
				dup2(out , STDOUT_FILENO);	
				close(out);
			}	
		}
		else
		{
			// ft_putstr_fd("error2\n", 2);
			// printf("3333333\n");
			tmp->pid = fork();
			if (tmp->pid == 0)
			{
				// printf("3  %d\n", y.i);
				if (tmp->next)
					close(tmp->fd[0]);
				if(cmd->fd_input == -1)
					exit(1);
				creat_files(tmp ,tmp2, y.i);
				ft_command(tmp);
			}
			// printf("4  %d\n", y.i);
			if (tmp->next)
				close(tmp->fd[1]);
			if (tmp2)
				close(tmp2->fd[0]);
		}
		tmp2 = tmp;
		tmp = tmp->next;		
		y.i +=1;
	}
	while (y.i > 0)
	{
		wait(NULL);
		y.i -= 1;
	}
}

char	**ft_all_paths(t_env *envp)
{
	while (envp)
	{
		if (ft_strnstr(envp->env, "PATH", 4))
			return (ft_split(envp->env + 5, ':'));
		envp = envp->next;
	}
	return (NULL);
}

void cmd_cd(t_cmd *cmd)
{
	char *ss;
	char *str;
	char cwd[1024];

	ss = NULL;
	while(cmd->env)
	{
		if (ft_strnstr(cmd->env->env, "PWD", 4))
		{
			// printf(">>>>%s\n",cmd->env->env);
			ss = cmd->env->env + 4;
			break;
		}
		cmd->env = cmd->env->next;
	}
	if (!ss)
	{
		printf("no pwd\n");
		return ;
	}
	ss = ft_strjoin_no_free(ss ,"/");
	str = ss;
	ss = ft_strjoin_no_free(ss, cmd->cmd[1]);
	free(str);
	printf("ss == %s\n", ss);
	if (!chdir(ss))
	{
		getcwd(cwd, sizeof(cwd));
		// printf("cwd>>>>%s\n",cwd);
		free(cmd->env->env);
		cmd->env->env = ft_strjoin_no_free("PWD=",cwd);
		// free(ss);
		// exit(0);
	}
	else
		perror("getcwd");
		// printf("saf\n");
}

void	exection(t_cmd *cmd)
{
	t_cmd *tmp;
	cmd->paths = get_path(cmd->env);
	tmp = cmd;
	// if (cmd->paths == NULL)
	// {
	// 	printf("hhhhh\n");
	// }
	while (tmp)
	{
		// if(tmp->cmd == ' ')
		// 	tmp = tmp->next;
		tmp->paths = cmd->paths;
		tmp = tmp->next;
	}
	if(cmd->cmd && !cmd->next && word_stop("cd", cmd->cmd[0]))
	{
		cmd_cd(cmd);
	}
	else
		ft_pipe(cmd);
	// int i;

	// i = 0;
	// while (cmd->paths[i])
	// {
	// 	free(cmd->cmd_path[i]);
	// 	i++;
	// }
}
