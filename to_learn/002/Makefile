CC = cc
#CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRC = main.c
NAME = ./a.out 

all : $(NAME)

$(NAME) : $(SRC)
	$(CC) -I /usr/local/include $(SRC) -L /usr/local/lib -lmlx -framework OpenGL -framework AppKit

#bonus: $(OBJS_BONUS) $(OBJS)
#	$(AR) $(NAME) $(OBJS_BONUS) $(OBJS)


clean : 
	$(RM) $(NAME)
	
fclean : clean
	$(RM) $(NAME)

re : fclean all

