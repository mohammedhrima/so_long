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
int WINDOW_WIDTH = 600;
int WINDOW_HEIGHT = 300;

static int KEY = RIGHT;

typedef struct s_var
{
	void *img;
	char *addr;
	void *mlx;
	void *win;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_var;

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

int is_all_wall(char *str)
{
	int i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] != '1')
			return (-1);
		i++;
	}
	return (0);
}

int map_is_valid(char *str)
{
	int i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] != '0' && str[i] != '1' && str[i] != 'C' && str[i] && str[i] != 'E' && str[i] != 'P')
			return (-1);
	}
	return (0);
}

static int first = 1;

int len = 5;
void draw_map(t_var *var)
{
	int fd = open("./img/007.xpm", O_RDONLY);
	char *str = get_next_line(fd);

	int y = 10;
	while (str)
	{
		int i = 0;
		int x = 10;
		while (str[i])
		{
			if (str[i] == '1')
				put_pixels(var, x, y, len, len, RED);
			if (str[i] == '0')
				put_pixels(var, x, y, len, len, BLUE);
			x += len;
			i++;
		}
		y += len;
		free(str);
		str = get_next_line(fd);
	}
	close(fd);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	first = 0;
}

static int a = 0;
static int b = 0;

int draw(t_var *var)
{
	draw_map(var);
	return (0);
}

int main(void)
{

	t_var var;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Hello world!");
	var.img = mlx_new_image(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	mlx_loop_hook(var.mlx, draw, &var);
	// mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	mlx_loop(var.mlx);
}