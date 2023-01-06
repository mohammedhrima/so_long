#include <mlx.h>
#define RED 0xff0000

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

int circle(t_var *var, int x_center, int y_center, int radius, int color)
{
	int x;
	int y;
	int X;
	int Y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			X = x_center - x;
			Y = y_center - y;
			if (X * X + Y * Y <= radius * radius)
			{
				printf("(%d, %d)\n", x, y);
				// put_one_pixel(var, x, y, color);
				put_pixels(var, x, y, 1, 1, color);
				// mlx_pixel_put(var->mlx, var->win, x,y ,color);
			}
			y++;
		}
		x++;
	}
	return (0);
}

int draw(t_var *var)
{
	circle(var, 100, 100, 50, RED);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	return (0);
}

int main(void)
{
	// void *mlx;
	//	void *mlx_win;
	t_var var;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 600, 300, "Hello world!");
	var.img = mlx_new_image(var.mlx, 600, 300);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	put_pixels(&var, 5, 5, 1, 1, RED);
	mlx_loop_hook(var.mlx, draw, &var);
	// mlx_put_image_to_window(var.mlx, var.win, var.img, 0, 0);
	mlx_loop(var.mlx);
}