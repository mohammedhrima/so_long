# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/31 09:34:27 by mhrima            #+#    #+#              #
#    Updated: 2023/01/05 09:52:20 by mhrima           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRC = x.c
NAME = ./so_long
LIB = ./Libft/libft.a
SAN = -fsanitize=address -g3
#add line that make the libft if doesn't exists

all : $(NAME)

$(LIB):
	make -C ./Libft
	make -C ./Libft clean

$(NAME) : $(SRC) $(LIB)
	$(CC) $(SRC) $(LIB)  -lmlx -framework OpenGL -framework AppKit -o so_long

san:
	$(CC) $(SRC) $(LIB) $(SAN) -lmlx -framework OpenGL -framework AppKit -o so_long

wall: 
	$(CC) $(CFLAGS) $(SAN) $(SRC) $(LIB) -lmlx -framework OpenGL -framework AppKit -o so_long

# -O2
#-fsanitize=address -g3

bonus: $(OBJS_BONUS) $(OBJS)
	$(AR) $(NAME) $(OBJS_BONUS) $(OBJS)

map:
	gcc mapgenerator2.c -fsanitize=address -g3 && ./a.out && rm -rf ./a.out

clean : 
	$(RM) $(NAME)
	
fclean : clean
	$(RM) $(NAME)

re : fclean all