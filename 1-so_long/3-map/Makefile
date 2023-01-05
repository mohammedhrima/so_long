# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/31 09:34:27 by mhrima            #+#    #+#              #
#    Updated: 2023/01/05 07:36:22 by mhrima           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
#CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRC = x.c
NAME = ./a.out 
LIB = ./Libft/libft.a
#add line that make the libft if doesn't exists

all : $(NAME)

$(NAME) : $(SRC)
	$(CC) $(SRC) $(LIB) -lmlx -framework OpenGL -framework AppKit 
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