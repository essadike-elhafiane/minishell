NAME = minishell
CC = cc -Wall -Wextra -Werror -I/goinfre/eelhafia/.brew/opt/readline/include -g
RM = rm -f

SRC = main.c parsing/lexer.c parsing/heredoc.c parsing/creat_inp_out_a.c \
	parsing/lexer_fun.c parsing/parser_1.c parsing/parser.c parsing/creat_files_cmd.c libft/ft_strlen.c libft/ft_substr.c \
	libft/ft_strjoin.c libft/ft_strjoin_exe.c libft/ft_itoa.c libft/ft_atoi.c \
	libft/ft_strjoin_no_free.c libft/ft_strdup.c libft/ft_strnstr.c libft/ft_split.c\
	libft/ft_isdigit.c libft/ft_isalpha.c libft/ft_putstr_fd.c libft/ft_putchar_fd.c \
	parsing/free/free.c parsing/check.c parsing/check_error.c parsing/free/free_1.c libft/ft_strchr.c exe/exection.c \
	exe/add_env.c libft/ft_strncmp.c

OBJ = $(SRC:.c=.o)

all:$(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline -L/goinfre/eelhafia/.brew/opt/readline/lib -lhistory -o $(NAME)

%o: %c minishell.h
	$(CC) -c $< -o $@

clean :
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all