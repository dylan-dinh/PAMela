SRC	=	main.c

OBJ	=	$(SRC:.c=.o)

NAME	=	/lib/x86_64-linux-gnu/security/pam_module.so

CC	=	gcc

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CC) -c -fpic $(SRC)
		sudo $(CC) --shared -o $(NAME) $(OBJ)

install:	install.sh all
		./create_container.sh
		sudo ./install.sh

uninstall:	uninstall.sh fclean
		sudo ./uninstall.sh

check:

test:

clean:
		rm -f $(OBJ)

fclean:		clean
		sudo rm -f $(NAME)

re:		fclean all

.PHONY:		all clean fclean re
