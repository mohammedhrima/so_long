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
int SCALE = 50;

static int KEY = RIGHT;

typedef struct s_var
{
	void *img;
	char *addr;
	void *mlx;
	void *win;
	char **map;
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

int arrlen(char **arr)
{
	int i = 0;
	while (arr && arr[i])
	{
		// printf("108 -> %s\n", arr[i]);
		i++;
	}
	return (i);
}

int map_width(const char *str)
{
	int i;

	i = 0;
	while (str && str[i] && str[i] != '\n')
		i++;
	return (i);
}

int is_wall(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] == '1')
		i++;
	if (str[i] == '\n' || str[i] == '\0')
		return (0);
	else
		return (-1);
}

void Free_arr(char **arr)
{
	int i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char **arrjoin(char **arr, char *str) // to verify
{
	char **res;

	res = malloc((arrlen(arr) + 2) * sizeof(char **));
	if (res == NULL)
		exit(-1);
	int i = 0;
	while (arr && arr[i])
	{
		res[i] = arr[i];
		i++;
	}
	res[i] = ft_strdup(str);
	res[i + 1] = NULL;
	free(arr);
	return (res);
}

char *read_line(int fd)
{
	char *str;
	char *tmp;
	char buffer[2];

	str = NULL;
	int r = read(fd, buffer, 1);
	while (r > 0 && !ft_strchr(buffer, '\n'))
	{
		buffer[r] = '\0';
		tmp = ft_strjoin(str, buffer);
		free(str);
		str = tmp;
		r = read(fd, buffer, 1);
	}
	buffer[r] = '\0';
	tmp = ft_strjoin(str, buffer);
	free(str);
	return (tmp);
}

int len = 5;
char **get_map()
{
	int fd = open("./img/007.xpm", O_RDONLY);
	char *str = read_line(fd);
	char **map = NULL;
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
	WINDOW_WIDTH = map_width(str);
	while (str && ft_strlen(str))
	{
		// printf("-> %s\n", str);
		if (map_contains_allowed_chars(str) == -1)
		{
			Free_arr(map);
			free(str);
			printf("1.map is invalid, verify walls\n");
			exit(-1);
		}
		if (map_contains_allowed_chars(str) == -2)
		{
			Free_arr(map);
			free(str);
			printf("map is invalid, it contains unknown characters\n");
			exit(-1);
		}
		if (map_width(str) != WINDOW_WIDTH)
		{
			Free_arr(map);
			free(str);
			printf("map is invalid, all lines, doesn't have the same width\n");
			exit(-1);
		}
		map = arrjoin(map, str);
		free(str);
		str = read_line(fd);
		WINDOW_HEIGHT++;
	}
	if (is_wall(map[WINDOW_HEIGHT - 1]) < 0)
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
	int X = 0;
	int Y = 0;
	int len;
	while (var->map[y])
	{
		x = 0;
		Y = SCALE * y;
		while (var->map[y][x])
		{
			X = SCALE * x;
			if (var->map[y][x] == '0')
			{
				if (first)
					printf("(%d, %d) BLUE\n", X, Y);
				put_pixels(var, X, Y, SCALE, SCALE, BLUE);
			}
			if (var->map[y][x] == '1')
			{
				if (first)
					printf("(%d, %d) RED\n", X, Y);
				put_pixels(var, X, Y, SCALE, SCALE, RED);
			}
			x++;
		}
		y++;
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