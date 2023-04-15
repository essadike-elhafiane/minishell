/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:38:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/15 16:08:09 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
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

typedef struct s_shell
{
	int				type;
	char			*s;
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
	int		j;
	int		b;
} t_stk;

void    	lexer(char *str, char **env);
t_shell		*init_data(char *ss, int type1);
void		fun_free(t_shell **a);
int			check_is_oper(char c);
int    		parser(t_shell *data, char **env);
#endif
