CC = cc
#CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

#SRC = 002.c
NAME = ./a.out 


all : $(NAME)

$(NAME) : $(SRC)
	$(CC) $(SRC) -lmlx -framework OpenGL -framework AppKit

bonus: $(OBJS_BONUS) $(OBJS)
	$(AR) $(NAME) $(OBJS_BONUS) $(OBJS)


clean : 
	$(RM) $(NAME)
	
fclean : clean
	$(RM) $(NAME)

re : fclean all