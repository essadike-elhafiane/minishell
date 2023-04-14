/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:38:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/14 18:32:01 by eelhafia         ###   ########.fr       */
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
	int     i;
	int		j;
	int		b;
	t_shell *data_cmd;
	t_shell *tmp;
	char *ss;
} t_stk;

void    	lexer(char *str);
t_shell		*init_data(char *ss, int type1);
void		fun_free(t_shell **a);
int			check_is_oper(char c);
void    	parser(t_shell *data);
#endif
