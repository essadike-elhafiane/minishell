/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/14 01:24:29 by eelhafia         ###   ########.fr       */
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
	char pwd[1024];

	j = 0;
	while(cmd->cmd[j])
	{
		i = 0;
		if (ft_strchr(cmd->cmd[j] ,'/'))
		{
			if (!access(cmd->cmd[j], X_OK))
			{
				cmd->cmd_path = ft_strdup (cmd->cmd[j]);
				return;
			}
			if (errno == EACCES)
			{
				perror("Minishell");
				exit(126);
        	} 
			else if (errno == ENOENT) 
			{
				perror("Minishell");
				exit(127);
        	} 
			
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
	ft_putstr_fd("➜ Minishell$: command not found\n", 2);
	exit(127);
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
	status = 0;
	exit(0);
}

void cmd_pwd(t_cmd *cmd)
{
	int i;
	char cd[1024];
	i = 0;

	while(*cmd->env)
	{
		if(ft_strnstr((*cmd->env)->env, "PWD", 3))
		{
			i = 1;
			printf("%s\n",(*cmd->env)->env + 4);
			break;
		}
		*cmd->env = (*cmd->env)->next;
	}
	if (i == 0)
		printf("%s\n",getcwd(cd , 1024));
	exit(0);
}


int hide_unset(t_env *tmp , char *cmd)
{
	if(!ft_strncmp(cmd,"HOME", 4) &&  !ft_strncmp (tmp->env,"HOME" , 4))
	{
		tmp->p = 0;
		return (1);
	}
	else if(!ft_strncmp(cmd,"PWD", 3) &&  !ft_strncmp (tmp->env,"PWD" , 3))
	{
		tmp->p = 0;
		return (1);
	}
	else if(!ft_strncmp(cmd,"OLDPWD", 6) &&  !ft_strncmp (tmp->env,"OLDPWD" , 6))
	{
		tmp->p = 0;
		return (1);
	}
	return (0);
}


void	cmd_unset(t_env **envs, t_cmd *cmd)
{
	t_env *tmp;
	t_env *tmp1;
	int		i;
	int len;

	i = 1;
	len = 0;
	while (cmd && cmd->cmd && cmd->cmd[i])
	{
		tmp = *envs;
		tmp1 = *envs;
		len = ft_strlen(cmd->cmd[i]);
		
		if ( tmp && !hide_unset(tmp , cmd->cmd[i]) && ft_strnstr(tmp->env, cmd->cmd[i], len) && (tmp->env[len] == '=' || tmp->env[len] == '\0') && tmp->env[0] != '?')
		{
			*envs = (*envs)->next;
			free(tmp->env);
			free(tmp);
			tmp = NULL;
		}
		while (tmp)
		{
			tmp1 = tmp;
			tmp = tmp->next;	
			if (tmp && !hide_unset(tmp , cmd->cmd[i]) && ft_strnstr(tmp->env, cmd->cmd[i], len) && (tmp->env[len] == '=' || tmp->env[len] == '\0') && tmp->env[0] != '?')
			{
				tmp1->next = tmp->next;
				free(tmp->env);
				free(tmp);
				break;
			}
		}
		i++;
	}
}

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

int  check_export(t_env **envs, t_cmd *cmd)
{
	t_env *tmp;
	int i;
	int plus;
	int a;
	int flg;
	int b;
	char *s;

	a = 1;
	if (!envs)
		printf("gdfdh\n");
	while(cmd && cmd->cmd[a])
	{
		flg = 0;
		i = -1;
		b = 0;
		plus = 0;
		tmp = *envs;
		i = ft_strcchr(cmd->cmd[a] ,'=');
		if (i > 0)
		{
			plus = ft_strcchr(cmd->cmd[a] ,'+');
			if (plus + 1 == i)
				i = plus;
		}
		s = ft_substr(cmd->cmd[a], 0, i);
		if (i < 0 && plus)
			b = -1;

		if (!cmd->cmd[a + 1] && cmd->cmd[a][0] == '\0')
			return (1);
		while (cmd->cmd[a] && cmd->cmd[a][b] && cmd->cmd[a][b] != '=')
		{
			if (cmd->cmd[a][b] == '+' && cmd->cmd[a][b + 1] == '=' && b > 0)
				b++;
			else if (ft_isdigit(cmd->cmd[a][0]) || (!ft_isalpha(cmd->cmd[a][b])
				&& !ft_isdigit(cmd->cmd[a][b]) && cmd->cmd[a][b] != '_' ))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(s, 2);
				ft_putstr_fd(" :not a valid identifier\n", 2);
				b = -1;
				break;
			}
			else
				b++;
		}
		while(tmp && cmd->cmd[a] && b > -1)
		{
			if ( i > 0 && ft_strnstr(tmp->env, s, ft_strlen(s)) && i != plus)
			{
				// if (tmp->export[ft_strlen(s) -1] != '\0' && tmp->export[ft_strlen(s) -1] != '=')
				// 	break;
				free(tmp->env);
				tmp->p = 1;
				tmp->env = ft_strdup(cmd->cmd[a]);
				flg = 1;
			}
			else if ( i > 0 && ft_strnstr(tmp->env, s, ft_strlen(s)) && i == plus)
			{
				if (ft_strcchr(tmp->env, '=') < 0)
					tmp->env = ft_strjoin(tmp->env, ft_strdup("="));
				tmp->p = 1;
				tmp->env =ft_strjoin(tmp->env, ft_strdup(cmd->cmd[a] + i + 2));
				flg = 1;
			}
			else if (ft_strnstr(tmp->env, cmd->cmd[a], ft_strlen(cmd->cmd[a])))
			{
				if (tmp->env[ft_strlen(s)] != '\0' && tmp->env[ft_strlen(s)] != '=')
					break;
				flg = 1;
			}
			tmp = tmp->next;
		}
		if(cmd->cmd[a] && i == plus && s && i > 0 && !flg)
		{
			free(cmd->cmd[a]);
			cmd->cmd[a] = ft_strjoin(ft_strdup(s), ft_strdup(cmd->cmd[a] + i + 1));
		}
		free(s);
		if (!flg && b > -1)
		{
			printf("%s", cmd->cmd[a]);
			cmd_export(cmd->env, cmd->cmd[a]);

		}
		a++;
	}
	return (0);
}

void	cmd_export_fork(t_env *envs, t_cmd *cmd)
{
	t_env *tmp;
	int i;
	int flg;
	
	tmp = envs;
	while(tmp)
	{
		i = -1;
		flg = 0;
		if (tmp->env[0] != '?' && tmp->p == 1)
			write(1, "declare -x ", 11);
		while (tmp->env[++i])
		{
			if (tmp->env[0] == '?' || tmp->p == 0)
				break;
			write(1, &tmp->env[i], 1);
			if (tmp->env[i] == '=' && !flg++)
				write(1 , "\"", 1);
		}
		if (flg && i != 0)
			write(1 , "\"\n", 2);
		else if (i != 0)
			write(1 , "\n", 1);
		tmp = tmp->next;
	}
	exit(0);
}

void	cmd_export(t_env **envs, char *cmd)
{
	t_env *tmp;
	t_env		*tmp2;

	if (!envs)
		return ;
	tmp = *envs;
	tmp2 = *envs;
	
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = (t_env *) malloc(sizeof(t_env));
	tmp = tmp->next;
	tmp->p = 1;
	tmp->next = NULL;
	tmp->env = ft_strdup(cmd);
}

void	cmd_env(t_env *envs)
{
	t_env *tmp;

	tmp = envs;
	while(tmp)
	{
		if (ft_strchr(tmp->env, '=') && tmp->env[0] != '?' && tmp->p != 0)
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
		cmd_export_fork(*cmd->env, cmd);
	else if(ft_strnstr (cmd->cmd[0], "env", 7))
		cmd_env(*cmd->env);
	else
	{
		ft_command_path(cmd ,cmd->paths);
		tab = envs_tab(*cmd->env);
		execve(cmd->cmd_path, cmd->cmd, tab);
		perror("Minishell");
		exit(126);
	}
}


void cmd_cd(t_cmd *cmd)
{
	char *ss;
	char *str;
	char *str3;
	char cwd[1024];
	t_env *tmp;
	static int flg;
	
	int g = 0;
	tmp = *cmd->env;
	ss = NULL;
	str3= NULL;
	while(tmp)
	{
		if (ft_strnstr(tmp->env, "PWD", 3)  && (tmp->env[3] == '=' || tmp->env[3] == '\0'))
		{
			ss = tmp->env + 4;
		}
		if (ft_strnstr(tmp->env, "HOME", 4) && (tmp->env[4] == '=' || tmp->env[4] == '\0'))
		{
			str3 = tmp->env + 5;
		}
		tmp = tmp->next;
	}
	if (cmd->cmd[1] == NULL || cmd->cmd[1][0] == '\0')
	{
		chdir(str3);
		status = 0;
	}
	else 
	{
		if ( chdir(cmd->cmd[1]))
		{
			perror("Minishell");
			status = 1;
			return ;
		}
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		printf("cd: error retrieving current directory: getcwd: \
			cannot access parent directories: No such file or directory\n");
			status = 0;
		if (!flg)
		{
			flg++;
			str =ft_strjoin(ft_strdup("OLDPWD="), ft_strdup(ss + 4));
		}
		ss = ft_strjoin(ft_strdup("PWD="), ft_strdup(ss));
		ss = ft_strjoin(ss, ft_strdup("/"));
		ss = ft_strjoin(ss, ft_strdup(cmd->cmd[1]));
	}
	else
	{
		if (ss[ft_strlen(ss) - 1] != '.')
			str = ft_strjoin(ft_strdup("OLDPWD="), ft_strdup(ss));
		ss = ft_strjoin_no_free("PWD=", getcwd(cwd, sizeof(cwd)));
		flg = 0;
		status = 0;
	}
	tmp = *cmd->env;
	while(tmp)
	{
		if (ft_strnstr(tmp->env, "PWD", 3))
		{
			free(tmp->env);
				// printf("%s\n",ss);
			tmp->env = ss;
			// if (ss)
		}
		if (ft_strnstr(tmp->env, "OLDPWD=", 8) && str != NULL)
		{
			if (!flg)
			{
				free(tmp->env);
				tmp->env = str;
			}
			else
				free(str);
		}
		tmp = tmp->next;
	}
	return ;
}



int is_not_fork(t_cmd *cmd)
{
	if (cmd->cmd && ft_strnstr(cmd->cmd[0], "export", 7))
	{
		if (!cmd->cmd[1])
		{
			// status = 0;
			return (0);
		}
		if (cmd->cmd[1] && cmd->cmd[1][0] == '\0')
		{
			ft_putstr_fd("minishell: export: `': not a valid identifier\n", 2);
			status = 1;
			return (1);
		}
		else
		{
			if (check_export(cmd->env , cmd))
				return (0);
			return (1);
		}
	}
	else if(cmd->cmd && cmd->cmd[0] && ft_strnstr (cmd->cmd[0], "cd", 2))
		return(1);
	else if (cmd->cmd && word_stop(cmd->cmd[0], "unset") && cmd->cmd[1] == NULL)
		return (1);
	else if (cmd->cmd && word_stop(cmd->cmd[0], "unset") && cmd->cmd[1] != NULL)
	{
		cmd_unset(cmd->env, cmd);
		return(1);
	}
	return (0);
}

void ft_pipe(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_stk	y;
	int input;
	int out;
	int j = 0;
	t_cmd *tmp2;
	int exs;
	int sta;
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
			
				j  = 0;
		}
		else
		{
			tmp->pid = fork();
			if (tmp->pid == 0)
			{
				// printf("3  %d\n", y.i);
				if (tmp->next)
					close(tmp->fd[0]);
				if(tmp->fd_input == -1)
				{
					printf("Minishell$: No such file or directory\n");
					exit(1);
				}
				creat_files(tmp ,tmp2, y.i);
				ft_command(tmp);
			}
			if (tmp->next)
				close(tmp->fd[1]);
			if (tmp2)
				close(tmp2->fd[0]);
		}
		tmp2 = tmp;
		tmp = tmp->next;		
		y.i +=1;
	}
	exs =  tmp2->pid;
	while (y.i > 0)
	{
		if (waitpid(-1 ,&sta, 0) == exs)
		{
			if (WIFEXITED(sta)) 
			{	
				status = WEXITSTATUS(sta);
			}
			 else if (WIFSIGNALED(sta)) 
			{
				status = WIFSIGNALED(sta);
				// printf("%d\n", status);
			}
		}
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

void	exection(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	cmd->paths = get_path(*cmd->env);
	tmp = cmd;
	while (tmp)
	{
		tmp->paths = cmd->paths;
		tmp = tmp->next;
	}
	if(cmd->cmd && !cmd->next && word_stop("cd", cmd->cmd[0]))
	{
		cmd_cd(cmd);
	}
	else
		ft_pipe(cmd);
	i = 0;
	while (cmd->paths && cmd->paths[i])
	{
		free(cmd->paths[i]);
		i++;
	}
	free(cmd->paths);
}
