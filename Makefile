SRC     =       crypt_folder.c

OBJ     =       $(SRC:.c=.o)

NAME    =       /lib/x86_64-linux-gnu/security/pam_module.so

CC      =       gcc

all:            $(NAME)

$(NAME):        $(OBJ)
                $(CC) -c -fpic $(SRC)
                sudo $(CC) --shared -o $(NAME) $(OBJ)

install:        installer.sh all
                ./encrypted_container.sh
                sudo ./installer.sh

uninstall:      uninstaller.sh fclean
                sudo ./uninstaller.sh

clean:
                rm -f $(OBJ)

fclean:         clean
                sudo rm -f $(NAME)

re:             fclean all

.PHONY:         all clean fclean re
