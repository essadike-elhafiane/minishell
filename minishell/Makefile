NAME = minishell
CC = CC 
RM = rm -f

SRC = main.c lexer.c parser.c libft.a libftt.a

OBJ = $(SRC:.c=.o)

all: $(lib) $(NAME)

$(NAME): $(lib) $(OBJ)
	$(CC) $(OBJ) -lreadline -o $(NAME)

$(lib):
	cd libft && make

%o: %c minishell.h
	$(CC) -c $<

clean :
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all