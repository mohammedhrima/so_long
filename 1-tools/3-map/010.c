#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// WINDOW SETTING
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
int SCALE = 20;

// typedef struct s_pce
// {
// 	int Player[2];
// 	int Exit[2];
// 	int *Collectibles[2];
// 	int Clen;
// } t_pce;

void ft_putchar(int c)
{
	write(1, &c, 1);
}

void ft_putstr(char *str)
{
	int i = 0;
	while (str && str[i])
	{
		write(1, str + i, 1);
		i++;
	}
}

int ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void *ft_calloc(size_t count, size_t size)
{
	void *arr;
	unsigned char *ptr;
	size_t i;

	if (count && size > SIZE_MAX / count)
		return (NULL);
	arr = (void *)malloc(count * size);
	if (!arr)
		return (NULL);
	i = 0;
	ptr = (unsigned char *)arr;
	while (i < count * size)
	{
		ptr[i] = 0;
		i++;
	}
	return (arr);
}

char *ft_strdup(const char *s)
{
	char *ptr;
	int i;

	ptr = (char *)ft_calloc((ft_strlen(s) + 1), sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s && s[i])
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char *join_char_to_str(char *str, char c)
{
	int i;
	char *res;

	i = 0;
	if (str)
		res = malloc((ft_strlen(str) + 1) * sizeof(char));
	else
		res = malloc(2 * sizeof(char));
	while (str && str[i])
	{
		res[i] = str[i];
		i++;
	}
	res[i] = c;
	i++;
	res[i] = '\0';
	// free(str);
	return (res);
}

char *ft_strjoin(char *s1, char *s2)
{
	char *str;
	int i;

	i = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	str = ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	if (!str)
	{
		write(1, "Error in strjoin\n", 18);
		return (NULL);
	}
	while (s1 && *s1)
		str[i++] = *s1++;
	while (s2 && *s2)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}

int move(int Player, int to_find, int w, int h, char **was_here)
{
	int x = Player % w;
	int y = Player / w;
	int n = -1;

	(*was_here)[Player] = 'P';
	if (Player == to_find)
		return (0);
	if (x < w - 1 && (*was_here)[y * w + x + 1] != '1' && (*was_here)[y * w + x + 1] != 'P' && n < 0)
		n = move(y * w + x + 1, to_find, w, h, was_here);
	if (x > 1 && (*was_here)[y * w + x - 1] != '1' && (*was_here)[y * w + x - 1] != 'P' && n < 0)
		n = move(y * w + x - 1, to_find, w, h, was_here);
	if (y < h - 1 && (*was_here)[(y + 1) * w + x] != '1' && (*was_here)[(y + 1) * w + x] != 'P' && n < 0)
		n = move((y + 1) * w + x, to_find, w, h, was_here);
	if (y > 1 && (*was_here)[(y - 1) * w + x] != '1' && (*was_here)[(y - 1) * w + x] != 'P' && n < 0)
		n = move((y - 1) * w + x, to_find, w, h, was_here);
	return (n);
}

char *build_string(char *arr[])
{
	int i = 0;
	char *str = NULL;
	char *tmp = NULL;

	while (arr && arr[i])
	{
		tmp = ft_strjoin(str, arr[i]);
		free(str);
		str = tmp;
		i++;
	}
	return (str);
}

int get_to_the_target(int w, int h, int Player, int Collectible, char *map)
{
	int i = 0;
	char *map_copy;

	map_copy = map;
	while (i < h)
	{
		int j = 0;
		while (j < w)
		{
			ft_putchar(map_copy[i * w + j]);
			j++;
		}
		ft_putchar('\n');
		i++;
	}
	ft_putchar('\n');
	int n = 0;
	n = move(Player, Collectible, w, h, &map_copy);
	if (n < 0)
		printf("Path Not found\n");
	else
		printf("Path is valid\n");

	ft_putchar('\n');
	i = 0;
	while (i < h)
	{
		int j = 0;
		while (j < w)
		{
			ft_putchar(map_copy[i * w + j]);
			j++;
		}
		ft_putchar('\n');
		i++;
	}
	ft_putstr("\n");
	return (n);
}

int is_wall(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] == '1')
		i++;
	if (str[i] == '\0')
		return (0);
	else
		return (-1);
}

char *read_line(int fd)
{
	char *str;
	char *tmp;
	char buffer[2];

	str = NULL;
	int r = read(fd, buffer, 1);
	while (r > 0 && buffer[0] != '\n')
	{
		buffer[r] = '\0';
		tmp = ft_strjoin(str, buffer);
		free(str);
		str = tmp;
		r = read(fd, buffer, 1);
	}
	return (str);
}

typedef struct s_game
{
	int Player;
	int Exit;
	int *Collectible;
	char *map;
} t_game;

int *join_2D(int *arr, int to_add)
{
	int *res;

	int i = 0;
	while (arr && arr[i])
		i++;
	res = malloc((i + 2) * sizeof(int));
	if (!res)
		exit(-1);
	i = 0;
	while (arr && arr[i])
	{
		res[i] = arr[i];
		i++;
	}
	res[i] = to_add;
	free(arr);
	i++;
	res[i] = 0;
	return (res);
}

int map_contains_allowed_chars(char *str, t_game *game, int w, int h)
{
	int i = 0;
	if (str[i] != '1')
		return (-1);
	while (str[i] && str[i] != '\n')
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != 'C' && str[i] && str[i] != 'E' && str[i] != 'P')
			return (-2);
		if (str[i] == 'P')
		{
			if (game->Player)
				return (-3);
			game->Player = h * w + i;
		}
		if (str[i] == 'C')
			game->Collectible = join_2D(game->Collectible, h * w + i);
		if (str[i] == 'E')
		{
			if (game->Exit)
				return (-4);
			game->Exit = h * w + i;
		}
		i++;
	}
	if (str[i - 1] != '1')
		return (-1);
	return (0);
}

/*
make this function return a struct that contains:
window height / width
number of Collect with there index
index of Player
number of exists with there index
*/

t_game get_map(int fd)
{ /*
  add getting index of Players and Collective and Exit here, and return theme as struct
  */
	t_game game;
	game.Player = 0;
	game.Exit = 0;
	game.Collectible = NULL;
	game.map = NULL;

	char *str = read_line(fd);
	WINDOW_WIDTH = 0;
	WINDOW_HEIGHT = 0;
	if (!str)
	{
		free(str);
		printf("something wrong with the map\n");
		exit(-1);
	}
	if (is_wall(str) < 0)
	{
		free(str);
		printf("0.map is invalid, verify walls\n");
		exit(-1);
	}
	WINDOW_WIDTH = ft_strlen(str);
	while (str && ft_strlen(str))
	{

		printf("-> %s\n", str);
		if (ft_strlen(str) != WINDOW_WIDTH)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, all lines, doesn't have the same width\n");
			exit(-1);
		}
		int n = map_contains_allowed_chars(str, &game, WINDOW_WIDTH, WINDOW_HEIGHT);
		if (n == -1)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("1.map is invalid, verify walls\n");
			exit(-1);
		}
		if (n == -2)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, it contains unknown characters\n");
			exit(-1);
		}
		if (n == -3)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, it contains more than one Player\n");
			exit(-1);
		}
		if (n == -4)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, it contains more than one Exit\n");
			exit(-1);
		}
		game.map = ft_strjoin(game.map, str);
		free(str);
		str = read_line(fd);
		WINDOW_HEIGHT++;
	}
	if (is_wall(game.map + WINDOW_HEIGHT * WINDOW_WIDTH - WINDOW_WIDTH) < 0)
	{
		free(game.map);
		printf("2.map is invalid, verify walls\n");
		exit(-1);
	}
	close(fd);
	return (game);
}

int main(void)
{
	// char *str = ft_calloc(255, sizeof(char));

	int fd = open("./img/009.xpm", O_RDONLY);
	t_game var = get_map(fd);
	int i = 0;
	printf("\n\nCollectible: ");
	while (var.Collectible[i])
	{
		printf("%d ", var.Collectible[i]);
		i++;
	}
	i = 0;
	printf("\nPlayer: %d\nExit: %d\n", var.Player, var.Exit);

	// char *map = var.map;
	//	printf("\nHeight: %d\nWidth: %d\nmap: %s\n", WINDOW_HEIGHT, WINDOW_WIDTH, map);
	// t_pce var;
	// int Player;
	// int Collectible;
	// int Exit;
	// int i = 0;
	// int j = 0;
	// while (j < WINDOW_HEIGHT)
	// {
	// 	i = 0;
	// 	while (i < WINDOW_WIDTH)
	// 	{
	// 		if (map[j * WINDOW_WIDTH + i] == 'P')
	// 		{
	// 			printf("Player (%d, %d)\n", i, j);
	// 			Player = j * WINDOW_WIDTH + i;
	// 		}
	// 		if (map[j * WINDOW_WIDTH + i] == 'C')
	// 		{
	// 			printf("Collectible (%d, %d)\n", i, j);
	// 			Collectible = j * WINDOW_WIDTH + i;
	// 		}
	// 		if (map[j * WINDOW_WIDTH + i] == 'E')
	// 		{
	// 			printf("Exit (%d, %d)\n", i, j);
	// 			Exit = j * WINDOW_WIDTH + i;
	// 		}
	// 		i++;
	// 	}
	// 	j++;
	// }
	// printf("=> %c\n", map[])
	// int n = get_to_the_target(i, j, Player, Collectible, map);
	//	printf("%d\n", n);
}