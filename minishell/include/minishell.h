/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:38:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/21 16:32:57 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include "../libft/libft.h"

# define WSPACE 'S'
# define WORD 'W'
# define DOUBLE 'D'
# define SINGLE 'N'
# define PIPE 'P'
# define IN 'I'
# define OUT 'O'
# define HER 'H'
# define APPEND 'A'

typedef struct s_env
{
	char			*env;
	int				p;
	struct s_env	*next;
}	t_env;

int	g_status;

typedef struct s_cmd
{
	char			**cmd;
	char			**paths;
	char			*cmd_path;
	t_env			**env;
	int				fd[2];
	pid_t			pid;
	int				fd_out;
	int				fd_input;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	char			*s;
	t_env			**env;
	int				type;
	int				var_re;
	int				len_spl;
	struct s_shell	*next;
}	t_shell;

typedef struct s_stk
{
	char	*ss;
	char	*back;
	char	*front;
	t_shell	*data_cmd;
	t_shell	*tmp;
	int		i;
	pid_t	pid;
	int		j;
	int		b;
	int		plus;
	int		flg;
	t_cmd	*tmp2;
	t_cmd	*tmp1;
}	t_stk;

void		signal_her(int signal);
void		free_double(char **spl);
int			ft_exit(char *s);
void		signal_handler(int signal);
void		lexer(char *str, t_env **envs);
t_shell		*init_data(char *ss, int type1);
void		fun_free(t_shell **a);
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
int			check_error(char *str);
int			check_is_token(int type);
int			check_error_help(char *str, int i);
int			check_double_oper(t_shell *data);
int			check_is_word_after_oper(t_shell *data);
int			checke_pipe(char *str);
int			check_is_oper(char c);
int			checke_double(char *str);
int			checke_single(char *str);
void		clean_cmd_help(t_cmd *cmd, t_stk	*y, char **spl, int flg);
void		lexer_help(t_stk *y, t_env **envs);
void		find_exit_status(t_env *env);
int			creat_heredoc(t_cmd **cmds, t_shell **tmp,
				t_stk *y, t_cmd *tmp_cmd);
void		creat_append(t_shell **tmp, t_cmd **tmp_cmd);
int			creat_inp_out(t_shell **tmp, t_cmd **tmp_cmd);
void		creat_word_cmd_h_1(t_cmd *tmp_cmd, t_shell **r, t_stk *y);
void		exection(t_cmd *cmd);
void		cmd_export(t_env **envs, char *cmd);
int			ft_strcchr(char *str, char c);
void		error_message(char *str, int i);
void		cmd_unset(t_env **envs, t_cmd *cmd);
void		cmd_cd(t_cmd *cmd);
void		ft_pipe(t_cmd *cmd);
int			check_export(t_env **envs, t_cmd *cmd);
void		cmd_unset(t_env **envs, t_cmd *cmd);
void		ft_command(t_cmd *cmd, t_cmd *tmp2, int i, t_stk y);
void		cmd_echo(t_cmd *cmd);
void		cmd_pwd(t_cmd *cmd);
void		cmd_env(t_env *envs);
void		cmd_export_fork(t_env *tmp);
int			ft_command_path_norm(t_cmd *cmd, int j, char **path);
#endif
