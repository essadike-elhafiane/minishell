/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:38:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/01 16:53:47 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "../libft/libft.h"

# define SPACE 'S'
# define WORD 'W'
# define DOUBLE 'D'
# define SINGLE 'N'
# define PIPE 'P'
# define IN 'I'
# define OUT 'O'
# define HER 'H'
# define APPEND 'A'



typedef struct s_export
{
	int				size;
	char			*export;
	struct s_export  *next;
	struct s_export  *head;
} t_export;

typedef struct s_env
{
	int				size;
	t_export		*export;
	char			*env;
	struct s_env  *next;
} t_env;


typedef struct s_cmd
{
	char			**cmd;
	char			**paths;
	char			*cmd_path;
	t_env			*env;
	int				fd[2];
	pid_t			pid;
	int				fd_out;
	int				fd_input;
	struct s_cmd 	*next;
} t_cmd;

typedef struct s_shell
{
	char			*s;
	t_env			*env;
	int				type;
	struct s_shell  *next;
} t_shell;

typedef struct s_stk
{
	char *ss;
	char *back;
	char *front;
	t_shell *data_cmd;
	t_shell *tmp;
	int     i;
	pid_t	pid;
	int		j;
	int		b;
} t_stk;

void		lexer(char *str, t_env *envs);
t_shell		*init_data(char *ss, int type1);
void		fun_free(t_shell **a);
int			check_is_oper(char c);
int			parser(t_shell *data, t_env *env);
void		fun_free_env(t_env **a);
t_cmd		*creat_cmd(t_shell *data);
void		fun_free_cmd(t_cmd **a);
void		clean_cmd(t_cmd *cmd);
int			word_stop(char *word, char *str);
t_env		*creat_env_list(char **env);
void		check_expand(t_stk *y, t_shell *tmp, t_env *env);
char		*expand(char *ss, t_env *env);
int			check_is_oper_error(char c);
int			check_is_token(int type);
void		clean_cmd_help(t_cmd *cmd, t_stk	*y, char **spl, int flg);

void		exection(t_cmd *cmd);
t_export	*creat_export(t_env *envs);
void		cmd_export(t_env *envs, char *cmd);
#endif
