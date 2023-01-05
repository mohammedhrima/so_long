#include <mlx.h>
#include <math.h>
#include <unistd.h>

// COLORS
#define RED 0xff0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define YELLOW 0xFFFF00
#define WHITE 0xFFFFFF

// WINDOW SETTING
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 300

// KEYS
#define RIGHT 124
#define DOWN 125
#define LEFT 123
#define UP 126

// SETTING
#define RADIUS 50

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

	if (x < 0)
		x += WINDOW_WIDTH;
	if (x > WINDOW_WIDTH)
		x -= WINDOW_WIDTH;
	if (y < 0)
		y += WINDOW_HEIGHT;
	if (y > WINDOW_HEIGHT)
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

int pow2(int num)
{
	return (num * num);
}

int head(t_var *var, int x_center, int y_center, int radius, int color)
{
	int x;
	int y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center)
		{
			if (pow2(x_center - x) + pow2(y_center - y) <= pow2(radius))
				put_pixels(var, x, y, 1, 1, color);
			y++;
		}
		x++;
	}
	return (0);
}

int draw_circle(t_var *var, int x_center, int y_center, int radius, int color)
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
				put_pixels(var, x, y, 1, 1, color);
			y++;
		}
		x++;
	}
	return (0);
}

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
			put_pixels(var, x_start + w, y_start + h, 1, 1, color);
			h++;
		}
		w++;
	}
	return (0);
}

static float frequence = 0.9;
// static float i = 0.1;
float j = 0.1;

void draw_enemies(t_var *var, int x, int y, int z, int radius)
{
	// frequence += j;
	// if (frequence == 1)
	// 	j = -0.1;
	// if (frequence < 0)
	// 	j = 0.1;
	// x += radius;
	// y += radius;
	// draw_circle(var, x, y, radius, 0);

	head(var, x + radius, y + radius, radius, YELLOW);
	draw_rect(var, x, y + radius, radius, 2 * radius, YELLOW);
	draw_circle(var, x + radius * 0.6, y + radius / 2, radius / 4, WHITE); // left eye
	draw_circle(var, x + radius * 1.4, y + radius / 2, radius / 4, WHITE); // right eye

	draw_circle(var, x + radius * 0.6 + z, y + radius / 2 + 2, radius / 8, 0); // left eye
	draw_circle(var, x + radius * 1.4 + z, y + radius / 2 + 2, radius / 8, 0); // right eye
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
}

static int a = 12;
static int b = 12;

static float c = 4;
float i = -0.5;

int draw(t_var *var)
{

	draw_rect(var, a - 1, b, 2 * RADIUS, 2 * RADIUS, 0);
	// mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	draw_enemies(var, a, b, c, RADIUS);
	c += i;
	if (c == 4)
		i = -0.5;
	if (c < -4)
		i = 0.5;
	a++;
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