# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/31 09:34:27 by mhrima            #+#    #+#              #
#    Updated: 2022/12/31 09:34:48 by mhrima           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
#CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRC = x.c
NAME = ./a.out 


all : $(NAME)

$(NAME) : $(SRC)
	$(CC) $(SRC) -lmlx -framework OpenGL -framework AppKit -fsanitize=address -g3
	./a.out
# -O2
#-fsanitize=address -g3

bonus: $(OBJS_BONUS) $(OBJS)
	$(AR) $(NAME) $(OBJS_BONUS) $(OBJS)


clean : 
	$(RM) $(NAME)
	
fclean : clean
	$(RM) $(NAME)

re : fclean all