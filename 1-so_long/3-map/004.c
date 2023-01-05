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

// SETTING
#define RADIUS 20

// WINDOW SETTING
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
int SCALE = 20;

static int KEY = RIGHT;

typedef struct s_var
{
	void *img;
	char *addr;
	void *mlx;
	void *win;
	char *map;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_var;

void put_one_pixel(t_var *var, int x, int y, int color)
{
	char *dst;

	while (x < 0)
		x += SCALE * WINDOW_WIDTH;
	while (x >= SCALE * WINDOW_WIDTH)
		x -= SCALE * WINDOW_WIDTH;
	while (y < 0)
		y += SCALE * WINDOW_HEIGHT;
	while (y >= SCALE * WINDOW_HEIGHT)
		y -= SCALE * WINDOW_HEIGHT;

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

int map_contains_allowed_chars(char *str)
{
	int i = 0;
	if (str[i] != '1')
		return (-1);
	while (str[i] && str[i] != '\n')
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != 'C' && str[i] && str[i] != 'E' && str[i] != 'P')
			return (-2);
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

int len = 5;
char *get_map()
{
	int fd = open("./img/007.xpm", O_RDONLY);
	char *str = read_line(fd);
	char *map = NULL;
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
		if (map_contains_allowed_chars(str) == -1)
		{
			free(str);
			printf("1.map is invalid, verify walls\n");
			exit(-1);
		}
		if (map_contains_allowed_chars(str) == -2)
		{
			free(str);
			printf("map is invalid, it contains unknown characters\n");
			exit(-1);
		}
		if (ft_strlen(str) != WINDOW_WIDTH)
		{
			free(map);
			free(str);
			printf("map is invalid, all lines, doesn't have the same width\n");
			exit(-1);
		}
		map = ft_strjoin(map, str);
		free(str);
		str = read_line(fd);
		WINDOW_HEIGHT++;
	}
	if (is_wall(map + WINDOW_HEIGHT * WINDOW_WIDTH - WINDOW_WIDTH) < 0)
	{
		free(map);
		printf("2.map is invalid, verify walls\n");
		exit(-1);
	}
	close(fd);
	return (map);
}

static int a = 0;
static int b = 0;
int first = 1;

int draw(t_var *var)
{
	int x = 0;
	int y = 0;
	int i = 0;
	int len;
	while (var->map[i])
	{
		x = i % WINDOW_WIDTH;
		y = i / WINDOW_WIDTH;
		if (var->map[i] == '0')
			put_pixels(var, SCALE * x, SCALE * y, SCALE, SCALE, 0);
		if (var->map[i] == '1')
			put_pixels(var, SCALE * x, SCALE * y, SCALE, SCALE, BLUE);
		if (var->map[i] == 'P')
			put_pixels(var, SCALE * x, SCALE * y, SCALE, SCALE, GREEN);
		if (var->map[i] == 'E')
			put_pixels(var, SCALE * x, SCALE * y, SCALE, SCALE, WHITE);
		if (var->map[i] == 'C')
			put_pixels(var, SCALE * x, SCALE * y, SCALE, SCALE, RED);
		i++;
	}
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	first = 0;
	return (0);
}

int main(void)
{
	t_var var;

	var.map = get_map();
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE, "Hello world!");
	var.img = mlx_new_image(var.mlx, WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	draw(&var);
	// mlx_loop_hook(var.mlx, draw, &var);
	//  // mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	mlx_loop(var.mlx);
}