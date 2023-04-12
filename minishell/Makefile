NAME = minishell
CC = CC -Wall -Wextra -Werror
RM = rm -f

SRC = main.c libft.a libftt.a

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline -o $(NAME)

%o : %c minishell.h
	$(CC) -c $<

clean :
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all