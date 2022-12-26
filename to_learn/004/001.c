#include <mlx.h>
#include <stdio.h>
#define WIN_WIDTH 520;
#define WIN_HEIGHT 520;

typedef struct s_vars
{
	void *mlx;
	void *win;
	int height;
	int width;
} t_vars;

int func()
{
	static int a;
	a++;
	printf("hello %d\n", a);
	return 0;
}

int draw_rect(t_vars *var, int x_start, int y_start, int x_end, int y_end, int color)
{
	int y;

	while (x_start < x_end)
	{
		y = y_start;
		while (y < y_end)
		{
			mlx_pixel_put(var->mlx, var->win, x_start, y, color);
			y++;
		}
		x_start++;
	}
	return (0);
}

int draw_moving_rect(t_vars *var)
{
	static int x = 10;
	static int y = 10;
	static int min_x = 10;
	static int min_y = 10;
	static int max_x = 510; // to see
	static int max_y = 520; // to see
	int rec_height = 50;
	int rec_width = 50;
	// int z;

	draw_rect(var, 0, 0, var->width, var->height, 0); // draw black rect
	printf("x: %d , y: %d\n", x, y);
	draw_rect(var, x, y, x + rec_width, y + rec_height, 0xff0000);
	if (x + rec_width < max_x && y == min_y)
	{
		x += 10;
		if (x + rec_width == max_x)
		{
			{
				if (max_y == 520)
					max_y -= 10;
				else
					max_y -= rec_height;
			}
		}
		return (0);
	}
	if (y + rec_height < max_y && x + rec_width == max_x)
	{
		y += 10;
		if (y + rec_height == max_y)
		{
			max_x -= rec_width;
		}
		return (0);
	}
	if (y + rec_height == max_y && x > min_x)
	{
		x -= 10;
		if (x == min_x)
		{
			min_y += rec_height;
		}
		return (0);
	}
	if (x == min_x && y > min_y)
	{
		y -= 10;
		if (y == min_y)
		{
			min_x += rec_width;
		}
		return (0);
	}
	return (-1);
}
int main(void)
{
	t_vars var;

	var.height = WIN_HEIGHT;
	var.width = WIN_WIDTH;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "test");
	mlx_key_hook(var.win, func, &var);				// mlx_key_hook call func when listening on an event
	mlx_loop_hook(var.mlx, draw_moving_rect, &var); // mlx loop hook keep calling the function
	mlx_loop(var.mlx);
}