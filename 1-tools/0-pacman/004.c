#include <mlx.h>
#include <math.h>
#include <unistd.h>

#define RED 0xff0000
#define BLUE0 0x000000FF
#define YELLOW 0xFFFF00

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
	int x_end = x_start;
	int y_end = y_start;

	while (x_end - x_start < width)
	{
		y_end = y_start;
		while (y_end - y_start < height)
		{
			// put_one_pixel(var, x_end, y_end, color);
			// or
			dst = var->addr + (y_end * var->line_length + x_end * (var->bits_per_pixel / 8));
			*(unsigned int *)dst = color;
			y_end++;
		}
		x_end++;
	}
}

static int break_points = 0;
int circle(t_var *var, int x_center, int y_center, int radius, int color, float frequence)
{
	int x;
	int y;
	int X;
	int Y;
	//	write(1, &frequence, sizeof(float));
	//	write(1," ",2);
	if (color == RED)
		printf("->%f \n", frequence);

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			X = x - x_center;
			Y = y - y_center;
			if (X * X + Y * Y <= radius * radius && !(X > 0 && Y > 0))
			{
				put_pixels(var, x, y, 1, 1, color);
			}
			y++;
		}
		x++;
	}
	x = x_center;
	while (x < x_center + radius)
	{
		y = y_center;
		while (y < y_center + radius)
		{
			X = x - x_center;
			Y = y - y_center;
			if (X * X + Y * Y <= radius * radius && (float)Y > X * frequence) // better not to do Y/X because X can be 0
			{
				put_pixels(var, x, y, 1, 1, color);
			}
			y++;
		}
		x++;
	}
	break_points = 1;
	return (0);
}

static float frequence = 0.9;
static float i = 0.1;

int draw(t_var *var)
{
	frequence += i;
	if (frequence == 1)
	{
		i = -0.1;
		write(1, "- \n", 4);
	}
	if (frequence < 0)
	{
		i = 0.1;
		write(1, "+ \n", 4);
	}
	if (frequence * 10 / 2 == 0)
		printf("is odd\n");
	circle(var, 100, 100, 50, 0, 0);
	circle(var, 100, 100, 50, YELLOW, frequence);
	//circle(var, 100, 75, 8, 0,0);

	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	// sleep(1);
	return (0);
}

int main(void)
{
	// void *mlx;
	//	void *mlx_win;
	//	printf("-> %f\n", sinf(45));
	t_var var;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 600, 300, "Hello world!");
	var.img = mlx_new_image(var.mlx, 600, 300);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	//put_pixels(&var, 5, 5, 1, 1, RED);
	mlx_loop_hook(var.mlx, draw, &var);
	// mlx_put_image_to_window(var.mlx, var.win, var.img, 0, 0);
	mlx_loop(var.mlx);
}