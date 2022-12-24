#include <mlx.h>
#include <stdio.h>

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
} t_var;

int func()
{
	printf("hey\n");
	return (0);
}

int circle(t_var *var, int x_center, int y_center, int radius, int color)
{
	int x;
	int y;

	x = x_center;
	while (x < x_center + radius)
	{
		y = y_center;
		while (y < y_center + radius)
		{
			if(radius * radius >= (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center))
				mlx_pixel_put(var->mlx, var->win, x, y, color);
			y++;
		}
		y = y_center;
		while (y > y_center - radius)
		{
			if(radius * radius >= (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center))
				mlx_pixel_put(var->mlx, var->win, x, y, color);
			y--;
		}
		x++;
	}
	x = x_center;
	while (x > x_center - radius)
	{
		y = y_center;
		while (y < y_center + radius)
		{
			if(radius * radius >= (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center))
				mlx_pixel_put(var->mlx, var->win, x, y, color);
			y++;
		}
		y = y_center;
		while (y > y_center - radius)
		{
			if(radius * radius >= (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center))
				mlx_pixel_put(var->mlx, var->win, x, y, color);
			y--;
		}
		x--;
	}
	return (0);
}

int draw(t_var *var)
{
	circle(var, var->width / 2, var->height / 2, 50, 0xff0000);
	return (0);
}

int main(void)
{
	t_var var;

	var.mlx = mlx_init();
	var.width = 600;
	var.height = 600;
	var.win = mlx_new_window(var.mlx, var.width, var.height, "window");
	// key hook: listen on events
	mlx_key_hook(var.win, func, &var);
	// loop: keep calling function;
	mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}