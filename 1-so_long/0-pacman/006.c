#include <mlx.h>
#include <math.h>
#include <unistd.h>

#define RED 0xff0000
#define BLUE0 0x000000FF
#define YELLOW 0xFFFF00
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 300

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
			// or
			// dst = var->addr + (y_end * var->line_length + x_end * (var->bits_per_pixel / 8));
			// *(unsigned int *)dst = color;
			y_end++;
		}
		x_end++;
	}
}

static int break_points = 0;

int abs(int num)
{
	if (num < 0)
		return -num;
	return num;
}

int circle(t_var *var, int x_center, int y_center, int radius, int color, float frequence)
{
	int x;
	int y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			if ((x - x_center) * (x - x_center) + (y - y_center) * (y - y_center) <= radius * radius && (x - x_center) < 0)
				put_pixels(var, x, y, 1, 1, color);
			y++;
		}
		x++;
	}
	x = x_center;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			if ((x - x_center) * (x - x_center) + (y - y_center) * (y - y_center) <= radius * radius && abs(y - y_center) >= (x - x_center) * frequence) // better not to do Y/X because X can be 0
				put_pixels(var, x, y, 1, 1, color);
			y++;
		}
		x++;
	}
	break_points = 1;
	return (0);
}

static float frequence = 0.9;
static float i = 0.1;
float j = 0.1;

int draw_rect(t_var *var, int x_start, int y_start, int height, int width, int color)
{
	int h;
	int w;

	w = 0;
	while (w < width)
	{
		h = 0;
		while (h < height)
		{
			put_one_pixel(var, x_start + w, y_start + h, color);
			h++;
		}
		w++;
	}
	return (0);
}

void draw_pacman(t_var *var, int x, int y, int radius)
{
	draw_rect(var, x, y, radius * 2, radius * 2, 0);
	frequence += j;
	if (frequence == 1)
		j = -0.1;
	if (frequence < 0)
		j = 0.1;
	x += radius;
	y += radius;
	circle(var, x, y, radius, 0, 0);
	circle(var, x, y, radius, YELLOW, frequence);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
}



static int a = 2;
static int b = 2;

int draw(t_var *var)
{
	// circle(var, a, b + 50, 50, RED, 0);
	//draw_rect(var, a, b, 50 * 2, 50 * 2, 0);
	draw_pacman(var, a, b, 50);
	a++;
	// circle(var, a, b + 50, 50, RED, 0);
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
	mlx_loop(var.mlx);
}