#include <mlx.h>
#include <math.h>
#include <unistd.h>
#include "Libft/libft.h"
#include <fcntl.h>
#include <stdio.h>

// COLORS
#define RED 0xff0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define YELLOW 0xFFFF00
#define WHITE 0xFFFFFF

// KEYS
#define RIGHT 124
#define DOWN 125
#define LEFT 123
#define UP 126
static int KEY = RIGHT;

// SETTING
#define RADIUS 20

// WINDOW SETTING
static int WINDOW_WIDTH;
static int WINDOW_HEIGHT;

// STRUCTURES
typedef struct s_game
{
	int Player;
	int Exit;
	int *Collectible;
	int Collectlen;
	char *map;
} t_game;

typedef struct s_var
{
	void *img;
	char *addr;
	void *mlx;
	void *win;
	int bits_per_pixel;
	int line_length;
	int endian;
	t_game game;
} t_var;

// MAP functions
void ft_putstr(char *str)
{
	int i = 0;
	while (str && str[i])
	{
		write(1, str + i, 1);
		i++;
	}
}
void ft_bzero(void *s, size_t n)
{
	size_t i;
	unsigned char *ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (ptr && i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
char *read_line(int fd)
{
	char *str;
	char *tmp;
	char buffer[2];

	ft_bzero(buffer, 2);
	str = NULL;
	int r = read(fd, buffer, 1);
	while (r > 0 && buffer[0] != '\n')
	{
		buffer[r] = '\0';
		tmp = ft_strjoin(str, buffer);
		ft_bzero(buffer, 2);
		free(str);
		str = tmp;
		r = read(fd, buffer, 1);
	}
	if (r > 0 && !str)
		str = ft_strdup(buffer);
	return (str);
}
int *join_2D(int *arr, int to_add, int len)
{
	int *res;
	int i;

	res = malloc((len + 1) * sizeof(int));
	if (!res)
		exit(-1);
	i = 0;
	while (i < len - 1)
	{
		res[i] = arr[i];
		i++;
	}
	res[i] = to_add;
	free(arr);
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
		{
			game->Collectlen++;
			game->Collectible = join_2D(game->Collectible, h * w + i, game->Collectlen);
		}
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
int is_wall(char *str)
{
	int i;

	i = 0;
	while (str && str[i] && str[i] == '1')
		i++;
	if (i > 0 && (str[i] == '\n' || str[i] == '\0'))
		return (0);
	else
		return (-1);
}
t_game get_map(int fd)
{
	t_game game;
	game.Player = 0;
	game.Exit = 0;
	game.Collectible = NULL;
	game.Collectlen = 0;
	game.map = NULL;
	char *tmp;

	char *str = read_line(fd);
	WINDOW_WIDTH = 0;
	WINDOW_HEIGHT = 0;
	char *arr[] = {
		"",
		"map is invalid, verify walls\n",
		"map is invalid, it contains unknown characters\n",
		"map is invalid, it contains more than one Player\n"
		"map is invalid, it contains more than one Exit\n"};
	if (!str)
	{
		free(str);
		printf("something wrong with the map\n");
		exit(-1);
	}
	if (is_wall(str) < 0)
	{
		printf("map is invalid, verify first line if is wall\n");
		free(str);
		exit(-1);
	}
	WINDOW_WIDTH = ft_strlen(str);
	while (str && ft_strlen(str))
	{
		if (ft_strlen(str) != WINDOW_WIDTH)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, all lines, doesn't have the same width\n");
			exit(-1);
		}
		int n = map_contains_allowed_chars(str, &game, WINDOW_WIDTH, WINDOW_HEIGHT);
		if (n == -1 || n == -2 || n == -3 || n == -4)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			ft_putstr(arr[-n]);
			exit(-1);
		}
		tmp = ft_strjoin(game.map, str);
		free(game.map);
		free(str);
		game.map = tmp;
		str = read_line(fd);
		if (str && str[0] == '\n')
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, there is two followed new lines\n");
			exit(-1);
		}
		WINDOW_HEIGHT++;
	}

	if (is_wall(game.map + WINDOW_HEIGHT * WINDOW_WIDTH - WINDOW_WIDTH) < 0)
	{
		printf("map is invalid, verify last line if is wall\n");
		free(game.map);
		exit(-1);
	}
	if (game.Collectlen == 0)
	{
		free(game.map);
		printf("map is invalid, there isn't enough Collectible\n");
		exit(-1);
	}
	if (game.Player == 0)
	{
		free(game.map);
		printf("map is invalid, there isn't any Player\n");
		exit(-1);
	}
	if (game.Exit == 0)
	{
		free(game.map);
		printf("map is invalid, there isn't any Exit\n");
		exit(-1);
	}

	close(fd);
	return (game);
}
int move(int Player, t_game game, int w, int h, char **was_here)
{
	int x = Player % w;
	int y = Player / w;
	int n = 0;
	int i = 0;

	(*was_here)[Player] = 'P';
	if ((*was_here)[game.Exit] != 'P')
		n = -1;
	while (i < game.Collectlen)
	{
		if ((*was_here)[game.Collectible[i++]] != 'P')
			n = -1;
	}
	if (x < w - 1 && (*was_here)[y * w + x + 1] != '1' && (*was_here)[y * w + x + 1] != 'P' && n < 0)
		n = move(y * w + x + 1, game, w, h, was_here);
	if (x > 1 && (*was_here)[y * w + x - 1] != '1' && (*was_here)[y * w + x - 1] != 'P' && n < 0)
		n = move(y * w + x - 1, game, w, h, was_here);
	if (y < h - 1 && (*was_here)[(y + 1) * w + x] != '1' && (*was_here)[(y + 1) * w + x] != 'P' && n < 0)
		n = move((y + 1) * w + x, game, w, h, was_here);
	if (y > 1 && (*was_here)[(y - 1) * w + x] != '1' && (*was_here)[(y - 1) * w + x] != 'P' && n < 0)
		n = move((y - 1) * w + x, game, w, h, was_here);
	return (n);
}
char *get_to_the_target(t_game game)
{
	int i = 0;
	char *map_copy;

	map_copy = ft_strdup(game.map);
	int n = 0;
	n = move(game.Player, game, WINDOW_WIDTH, WINDOW_HEIGHT, &map_copy);
	if (n < 0)
		printf("Path is not valid\n");
	else
		printf("Path is valid\n");
	return (map_copy);
}

// DRAWING FUNCTIONS
void put_one_pixel(t_var *var, int x, int y, int color)
{
	char *dst;

	if (x >= WINDOW_WIDTH)
		x = x % WINDOW_WIDTH;
	if (x < 0)
		x = WINDOW_WIDTH + x % WINDOW_WIDTH;
	if (y >= WINDOW_HEIGHT)
		y = y % WINDOW_WIDTH;
	if (y < 0)
		y = WINDOW_HEIGHT + y % WINDOW_HEIGHT;

	dst = var->addr + (y * var->line_length + x * (var->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
void put_pixels(t_var *var, int x_start, int y_start, int width, int height, int color)
{
	char *dst;
	int x_end = x_start;
	int y_end = y_start;

	while (x_end - x_start < width)
	{
		y_end = y_start;
		while (y_end - y_start < height)
		{
			put_one_pixel(var, x_end, y_end, color);
			y_end++;
		}
		x_end++;
	}
}
int first = 1;
void draw_map(t_var *var)
{
	if (first)
	{
		printf("%s\n", var->game.map);
		first = 0;
	}
	int i = 0;
	int j = 0;
	while (i < WINDOW_HEIGHT)
	{
		j = 0;
		while (j < WINDOW_WIDTH)
		{
			if (var->game.map[i * WINDOW_WIDTH + j] == '1')
				put_one_pixel(var, j, i, BLUE);
			if (var->game.map[i * WINDOW_WIDTH + j] == 'P')
				put_one_pixel(var, j, i, GREEN);
			if (var->game.map[i * WINDOW_WIDTH + j] == 'E')
				put_one_pixel(var, j, i, 0);
			if (var->game.map[i * WINDOW_WIDTH + j] == '0')
				put_one_pixel(var, j, i, WHITE);
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
}
int draw(t_var *var)
{
	draw_map(var);
	return (0);
}
int main(void)
{
	t_var var;
	// t_game game;
	int fd;
	char *str;

	// open map and get the index of wanted elements
	fd = open("./map.ber", O_RDONLY);
	var.game = get_map(fd);
	int i = 0;
	printf("\nCollectible: ");
	while (i < var.game.Collectlen)
	{
		printf("%d ", var.game.Collectible[i]);
		i++;
	}
	printf("\nPlayer: %d\nExit: %d\n", var.game.Player, var.game.Exit);
	printf("Height: %d\nWidth: %d\n\n", WINDOW_HEIGHT, WINDOW_WIDTH);

	// check if there is valid path
	str = get_to_the_target(var.game);
	printf("\n");

	// i = 0;
	// int j = 0;
	// while (i < WINDOW_HEIGHT)
	// {
	// 	j = 0;
	// 	while (j < WINDOW_WIDTH)
	// 	{
	// 		printf("%c", str[i * WINDOW_WIDTH + j]);
	// 		j++;
	// 	}
	// 	printf("\n");
	// 	i++;
	// }

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 2 * WINDOW_WIDTH, 2 * WINDOW_HEIGHT, "Hello world!");
	var.img = mlx_new_image(var.mlx, 2 * WINDOW_WIDTH, 2 * WINDOW_HEIGHT);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	mlx_loop_hook(var.mlx, draw, &var);
	// mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	mlx_loop(var.mlx);
}