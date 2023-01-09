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
#define ORANGE 0xed6a12
#define YELLOW 0xFFFF00
#define BACKGROUND 0X0cf3ef
#define WHITE 0xFFFFFF

#define PI 3.14159
// WINDOW SETTING
static int WINDOW_WIDTH;
static int WINDOW_HEIGHT;

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
	dst = var->addr + (y * var->line_length + x * (var->bits_per_pixel / 8));

	*(unsigned int *)dst = color;
}
void put_pixels(t_var *var, int x_start, int y_start, int width, int height, int color)
{
	char *dst;
	int x_end = x_start + width;
	int y_end = y_start + height;

	if (x_start < 0)
		x_start = 0;
	if (x_end > WINDOW_WIDTH)
		x_end = WINDOW_WIDTH;
	if (y_start < 0)
		y_start = 0;
	if (y_end > WINDOW_HEIGHT)
		y_end = WINDOW_HEIGHT;
	// width--;
	// height--;

	for (int y = y_start; y < y_end; y++)
	{
		for (int x = x_start; x < x_end; x++)
		{
			put_one_pixel(var, x, y, color);
		}
	}
}
int abs(int num)
{
	if (num < 0)
		return -num;
	return num;
}

int pow2(int num)
{
	return (num * num);
}

float rad_to_deg(float rad)
{
	return ((rad / 180) * PI);
}

int face1(t_var *var, int x_center, int y_center, int radius, int color, int angle, int is_left)
{
	int x;
	int y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			if (pow2(x_center - x) + pow2(y_center - y) <= pow2(radius))
			{
				if ((is_left && x <= x_center) || (!is_left && x >= x_center))
					put_one_pixel(var, x, y, RED);
				else
					put_one_pixel(var, x, y, YELLOW);
				if (abs(y_center - y) >= sin(rad_to_deg(angle)) * abs(x_center - x))
					put_one_pixel(var, x, y, YELLOW);
			}
			y++;
		}
		x++;
	}
	return (0);
}
int pacman(t_var *var, int x_center, int y_center, int radius, int color, int angle, int dest)
{
	int x;
	int y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			if (pow2(x_center - x) + pow2(y_center - y) <= pow2(radius))
			{
				if ((dest == 'u' && y <= y_center) || (dest == 'd' && y >= y_center) || (dest == 'r' && x >= x_center) || (dest == 'l' && x <= x_center))
					put_one_pixel(var, x, y, 0);
				else
					put_one_pixel(var, x, y, YELLOW);
				if (((dest == 'u' || dest == 'd') && (abs(x_center - x) >= sin(rad_to_deg(angle)) * abs(y_center - y))) ||
					((dest == 'r' || dest == 'l') && abs(y_center - y) >= sin(rad_to_deg(angle)) * abs(x_center - x)))
					put_one_pixel(var, x, y, YELLOW);
			}
			y++;
		}
		x++;
	}
	return (0);
}

int draw(t_var *var)
{
	mlx_clear_window(var->mlx, var->win);
	static float angle = 20;
	static float q = 15;
	pacman(var, WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4, YELLOW, angle, 'u'); // is right
	// face2(var, WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4, YELLOW, angle);

	// circle(var, WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4, YELLOW, angle);
	if ((int)angle < 1 || (int)angle > 100)
		q = -q;

	angle += q;

	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	return (0);
}

int main(void)
{
	t_var var;
	// t_game game;
	int fd;
	char *str;
	WINDOW_HEIGHT = 300;
	WINDOW_WIDTH = 300;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Pacman");
	var.img = mlx_new_image(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}