NAME = minishell
CC = cc -g
RM = rm -f

SRC = main.c parsing/lexer.c parsing/parser.c parsing/creat_files_cmd.c libft/ft_strlen.c libft/ft_substr.c \
	libft/ft_strjoin.c libft/ft_strjoin_exe.c libft/ft_strjoin_no_free.c libft/ft_strdup.c libft/ft_strnstr.c libft/ft_split.c\
	libft/ft_isdigit.c libft/ft_isalpha.c libft/ft_putstr_fd.c libft/ft_putchar_fd.c \
	parsing/free/free.c libft/ft_strchr.c exe/exection.c libft/ft_strncmp.c

FILES = ft_striteri.c ft_memcmp.c  ft_bzero.c ft_putnbr_fd.c ft_strmapi.c ft_itoa.c \
	ft_putendl_fd.c ft_putchar_fd.c ft_putstr_fd.c ft_strtrim.c   \
	ft_calloc.c  ft_isalnum.c  \
	ft_isascii.c  ft_isprint.c \
	ft_memchr.c ft_memset.c ft_memcpy.c \
	 ft_strncmp.c ft_strlcat.c ft_strlcpy.c ft_atoi.c \
	ft_strchr.c ft_strrchr.c ft_strnstr.c \
	ft_tolower.c ft_toupper.c ft_memmove.c ft_split.c \
	

B_FILES = ft_lstiter.c ft_lstclear.c ft_lstdelone.c ft_lstlast.c ft_lstsize.c \
	ft_lstadd_back.c ft_lstadd_front.c ft_lstnew.c

OBJ = $(SRC:.c=.o)

all:$(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline -o $(NAME)

%o: %c minishell.h
	$(CC) -c $< -o $@

clean :
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all