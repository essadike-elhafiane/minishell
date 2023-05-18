/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:06:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/18 19:13:30 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_shell	*init_expand_space(t_shell *tmp, int i, char **spl)
{
	while (spl[i])
	{
		tmp->next = malloc(sizeof(t_shell));
		tmp = tmp->next;
		tmp->s = ft_strdup(spl[i]);
		tmp->type = WORD;
		tmp->var_re = 1;
		tmp->next = NULL;
		i++;
		if (spl[i])
		{
			tmp->next = malloc(sizeof(t_shell));
			tmp = tmp->next;
			tmp->s = ft_strdup(" ");
			tmp->type = WSPACE;
			tmp->var_re = 1;
			tmp->next = NULL;
		}
	}
	return (tmp);
}

void	check_space_exp(t_shell *tmp)
{
	t_shell	*tmp2;
	t_shell	*tmp1;
	char	**spl;
	int		i;

	i = 1;
	tmp1 = tmp;
	tmp->var_re = 1;
	tmp2 = tmp->next;
	spl = ft_split(tmp->s, ' ');
	free(tmp->s);
	tmp->s = ft_strdup(spl[0]);
	if (spl[i])
	{
		tmp->next = malloc(sizeof(t_shell));
		tmp = tmp->next;
		tmp->s = ft_strdup(" ");
		tmp->type = WSPACE;
		tmp->var_re = 1;
		tmp->next = NULL;
	}
	tmp = init_expand_space(tmp, i, spl);
	free_double(spl);
	tmp1->len_spl = i;
	tmp->next = tmp2;
}

void	check_expand_help(t_stk *y, t_shell *tmp, t_env *env, int flg)
{
	y->i++;
	y->b = y->i;
	while (ft_isalpha(tmp->s[y->i]) || ft_isdigit(tmp->s[y->i])
		|| tmp->s[y->i] == '_' || tmp->s[y->i] == '?')
		y->i++;
	y->ss = ft_substr(tmp->s, y->b, y->i - y->b);
	y->ss = expand(y->ss, env);
	if (!y->ss)
	{
		tmp->var_re = 1;
		tmp->len_spl = 2;
	}
	y->back = ft_strdup(tmp->s + y->i);
	if (!y->ss && tmp->s[y->i] && y->back)
		y->i = 0;
	y->front = ft_substr(tmp->s, 0, y->b -1);
	free(tmp->s);
	tmp->s = ft_strjoin(y->front, y->ss);
	if (tmp->s && tmp->s[y->i] && y->i > 1 && tmp->s[y->i - 1] != '$')
		y->i--;
	tmp->s = ft_strjoin(tmp->s, y->back);
	flg = 1;
}

void	check_expand(t_stk *y, t_shell *tmp, t_env *env)
{
	int	flg;

	flg = 0;
	y->i = 0;
	if (tmp->type == SINGLE)
		return ;
	while (tmp && tmp->s && tmp->s[y->i])
	{
		while (tmp->s[y->i] == '$' && tmp->s[y->i + 1] == '$')
			y->i += 2;
		if (tmp->s[y->i] == '$' && tmp->s[y->i +1] != '\0'
			&& (tmp->s[y->i + 1] != '$' && tmp->s[y->i + 1] != ' '))
		{
			check_expand_help(y, tmp, env, flg);
			flg = 1;
		}
		else
			y->i++;
	}
	if (flg && tmp->type != DOUBLE && tmp->s && ft_strchr(tmp->s, ' '))
		check_space_exp(tmp);
}

int	parser(t_shell *data, t_env *env)
{
	t_stk	y;
	t_shell	*tmp;

	tmp = data;
	while (tmp)
	{
		y.i = 0;
		if (tmp->type == HER)
		{
			tmp = tmp->next;
			if (tmp && tmp->type == WSPACE)
				tmp = tmp->next;
			while (tmp && !check_is_token(tmp->type))
				tmp = tmp->next;
		}
		if (tmp && tmp->s)
			check_expand(&y, tmp, env);
		if (tmp)
			tmp = tmp->next;
	}
	if (check_double_oper(data))
		return (1);
	if (check_is_word_after_oper(data))
		return (1);
	return (0);
}
