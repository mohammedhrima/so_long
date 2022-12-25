#include <mlx.h>
#include <stdio.h>

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
	// int z;

	draw_rect(var, 0, 0, var->width, var->height, 0); // draw black rect

	draw_rect(var, x, y, x + 50, y + 50, 0xff0000);
	if (x + 50 < var->width - 10 && y == 10)
	{
		x += 10;
		return (0);
	}
	if(y + 50 < var->height - 10 && x + 50 == var->width - 10)
	{
		y += 10;
		return (0);
	}
	if(y + 50 == var->height - 10 && x > 10)
	{
		x -= 10;
		return (0);
	}
	if(x == 10 && y > 10)
	{
		y -= 10;
		return (0);
	}
	return(-1);
}
int main(void)
{
	t_vars var;

	var.height = 600;
	var.width = 1000;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "test");
	mlx_key_hook(var.win, func, &var);				// mlx_key_hook call func when listening on an event
	mlx_loop_hook(var.mlx, draw_moving_rect, &var); // mlx loop hook keep calling the function
	mlx_loop(var.mlx);
}