#include <mlx.h>
#include <stdio.h>
#include <math.h>

// COLORS
#define RED 0xff0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define YELLOW 0xFFFF00

// WINDOW SETTING
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 300

#define PI 3.14159

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

int clicked_key()
{
	printf("hey\n");
	return (0);
}

void put_one_pixel(t_var *var, int x, int y, int color)
{
	char *dst;

	while (x < 0)
		x += WINDOW_WIDTH;
	while (x >= WINDOW_WIDTH)
		x -= WINDOW_WIDTH;
	while (y < 0)
		y += WINDOW_HEIGHT;
	while (y >= WINDOW_HEIGHT)
		y -= WINDOW_HEIGHT;

	dst = var->addr + (y * var->line_length + x * (var->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

float absolute(float num)
{
	if (num < 0)
		return -num;
	return num;
}

float deg_to_rad(float angle)
{
	return (angle * PI / 180);
}

int circle(t_var *var, int x_center, int y_center, int radius, int angle, int color)
{
	int x;
	int y;
	int X;
	int Y;

	x = x_center; // 0 in X
	while (x < x_center + radius)
	{
		y = y_center; // 0 in Y
		while (y > y_center - radius)
		{
			// right top
			X = x - x_center;
			Y = y_center - y;
			if (Y <= X * sin(deg_to_rad(angle)))
				put_one_pixel(var, x, y, color);
			y--;
		}
		x++;
	}
	return (0);
}

int line(t_var *var, int x_center, int y_center, int size, int angle)
{
	int x;
	int y;

	x = x_center + size;
	while (x > x_center)
	{
		y = y_center;
		while (y > y_center - size)
		{
			int X = x - x_center;
			int Y = y_center - y;
			if (Y == X * sin(deg_to_rad(angle)))
			{
				put_one_pixel(var, x, y, RED);
			}
			y--;
		}

		x--;
	}

	return (0);
}

int draw(t_var *var)
{
	// circle(var, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 50, 45, RED);
	line(var, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 20, 45);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	return (0);
}

int main(void)
{
	t_var var;
	// printf("-> %f\n",cos(deg_to_rad(45)));
	var.mlx = mlx_init();

	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "window");
	var.img = mlx_new_image(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);
	// key hook: listen on events
	mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	// loop: keep calling function;
	draw(&var);
	// mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}